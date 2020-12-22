#include "EditorLayer.h"

namespace CrashEngine {

	Editor::Editor()
		: Layer("Example")
	{
		//auto window = Application::Get().GetWindow();


		// initialize default shapes
		sphere.reset(new Sphere());
		square.reset(new Square());
		quad.reset(new Quad());

		float Height = Application::Get().GetWindow().GetHeight();
		float Width = Application::Get().GetWindow().GetWidth();

		cameraController.reset(new CameraController(glm::vec3(0.0f, 0.0f, 3.0f), Width, Height));


		imguilayer.reset(new ImGuiLayer);

		glm::mat4 projection = cameraController->GetCamera().GetProjectionMatrix();


		pbrShader = Shader::Create("pbr.vert", "pbr.frag");
		pbrShader->Bind();

		pbrShader->SetUniformVec3("albedo", glm::vec3(0.3f, 0.3f, 0.3f));
		pbrShader->SetUniformFloat("ao", 1.0f);
		pbrShader->SetUniformInt("irradianceMap", 0);
		pbrShader->SetUniformInt("prefilterMap", 1);
		pbrShader->SetUniformInt("brdfLUT", 2);

		equirectangularToCubemapShader = Shader::Create("cubemap.vert", "equirectangularToCubemap.frag");
		irradianceShader = Shader::Create("cubemap.vert", "irradiance.frag");
		prefilterShader = Shader::Create("cubemap.vert", "prefilter.frag");
		brdfShader = Shader::Create("brdf.vert", "brdf.frag");

		backgroundShader = Shader::Create("background.vert", "background.frag");
		backgroundShader->Bind();
		backgroundShader->SetUniformInt("environmentMap", 0);

		basicShader = Shader::Create("Basic.vert", "Basic.frag");


		pbrTextureShader = Shader::Create("pbr.vert", "pbrTexture.frag");
		pbrTextureShader->Bind();


		pbrTextureShader->SetUniformInt("albedoMap", 0);
		pbrTextureShader->SetUniformInt("normalMap", 1);
		pbrTextureShader->SetUniformInt("metallicMap", 2);
		pbrTextureShader->SetUniformInt("roughnessMap", 3);
		pbrTextureShader->SetUniformInt("aoMap", 4);

		pbrTextureShader->SetUniformInt("irradianceMap", 5);
		pbrTextureShader->SetUniformInt("prefilterMap", 6);
		pbrTextureShader->SetUniformInt("brdfLUT", 7);





		// pbr: setup framebuffer
		FramebufferSpecification spec;
		spec.Height = 1080;
		spec.Width = 2560;

		renderFramebuffer = Framebuffer::Create(spec);

		Framebuffer = Framebuffer::Create(spec);
		Framebuffer->Bind();


		Renderbuffer = Renderbuffer::Create();
		Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, 2560, 1080);
		Renderbuffer->AttachToFramebuffer();



		// set default settings
		RenderCommand::Enable(CE_DEPTH_TEST);
		RenderCommand::Enable(CE_TEXTURE_CUBE_MAP_SEAMLESS);
		RenderCommand::DepthFunc(CE_LEQUAL);


		//----------------------------Here should be func that loads hdr and calcualate maps--------------------------------
		HDR = TextureHDR::Create("hdr\\14-Hamarikyu_Bridge_B_3k.hdr");

		// pbr: setup cubemap to render to and attach to framebuffer
		Cubemap = CubemapTexture::Create(1080, 1080, true);


		// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};


		// pbr: convert HDR equirectangular environment map to cubemap equivalent
		equirectangularToCubemapShader->Bind();
		equirectangularToCubemapShader->SetUniformInt("equirectangularMap", 0);
		equirectangularToCubemapShader->SetUniformMat4("projection", captureProjection);
		RenderCommand::BindTexture(HDR->GetRendererID(), 0);

		RenderCommand::SetViewport(1080, 1080);
		Framebuffer->Bind();
		for (unsigned int i = 0; i < 6; ++i)
		{
			equirectangularToCubemapShader->SetUniformMat4("view", captureViews[i]);
			Framebuffer->SetTexture(CE_TEXTURE_CUBE_MAP_POSITIVE_X + i, Cubemap->GetRendererID(), 0);
			RenderCommand::Clear();

			square->RenderSquare();
		}
		Framebuffer->Unbind();

		// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
		Cubemap->CreateMipmap();

		// pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
		Irradiancemap = CubemapTexture::Create(32, 32, false);
		Framebuffer->Bind();
		Renderbuffer->Bind();
		Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, 32, 32);


		// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
		irradianceShader->Bind();
		irradianceShader->SetUniformInt("environmentMap", 0);
		irradianceShader->SetUniformMat4("projection", captureProjection);
		RenderCommand::BindCubemap(Cubemap->GetRendererID(), 0);


		RenderCommand::SetViewport(32, 32); // don't forget to configure the viewport to the capture dimensions.
		Framebuffer->Bind();
		for (unsigned int i = 0; i < 6; ++i)
		{
			irradianceShader->SetUniformMat4("view", captureViews[i]);
			Framebuffer->SetTexture(CE_TEXTURE_CUBE_MAP_POSITIVE_X + i, Irradiancemap->GetRendererID(), 0);
			RenderCommand::Clear();
			square->RenderSquare();
		}
		Framebuffer->Unbind();



		// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
		Prefiltermap = CubemapTexture::Create(128, 128, true);
		Prefiltermap->CreateMipmap();

		// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
		prefilterShader->Bind();
		prefilterShader->SetUniformInt("environmentMap", 0);
		prefilterShader->SetUniformMat4("projection", captureProjection);
		RenderCommand::BindCubemap(Cubemap->GetRendererID(), 0);

		Framebuffer->Bind();
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);
			Renderbuffer->Bind();
			Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, mipWidth, mipHeight);
			RenderCommand::SetViewport(mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			prefilterShader->SetUniformFloat("roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				prefilterShader->SetUniformMat4("view", captureViews[i]);
				Framebuffer->SetTexture(CE_TEXTURE_CUBE_MAP_POSITIVE_X + i, Prefiltermap->GetRendererID(), mip);

				RenderCommand::Clear();
				square->RenderSquare();
			}
		}
		Framebuffer->Unbind();


		texCreat = new TextureCreator();
		brdfLUTTexture = texCreat->CreateTexture(512, 512, brdfShader);



		UniformBufferLayout uniformLayout = {
			{ ShaderDataType::Mat4, "projection" },
			{ ShaderDataType::Mat4, "view"},
		};

		m_MatrixUB.reset(UniformBuffer::Create(uniformLayout, 0));
		m_MatrixUB->linkShader(pbrShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(pbrTextureShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(backgroundShader->GetID(), "Matrices");

		m_MatrixUB->setData("projection", glm::value_ptr(projection));


		RenderCommand::SetViewport(Width, Height);

	//-------------------------End of initialize-----------------------------------------


		//Scene and entities----------------------------------
		m_ActiveScene = std::make_shared<Scene>();

		//auto mesh1 = m_ActiveScene->CreateEntity("Mesh #1");
		//mesh1.AddComponent<MeshComponent>(testModel);
		//mesh1.GetComponent<TransformComponent>().Translation = glm::vec3(0,0,0);


		m_ActiveScene->SetDefaultShader(pbrTextureShader);

		auto view = m_ActiveScene->m_Registry.view<TransformComponent, MeshComponent>();
	
		CE_CORE_INFO("size of view in scene: {0}", view.size());

		HierarchyPanel = new SceneHierarchyPanel(m_ActiveScene);

		//m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void Editor::OnUpdate(Timestep ts)
	{
		cameraController->OnUpdate(ts);

		renderFramebuffer->Bind();

		RenderCommand::Clear();
		RenderCommand::SetClearColor({ 1.f, 0.f, 0.0f, 1.0f });

		Renderer::BeginScene();

		glm::mat4 view = cameraController->GetCamera().GetViewMatrix();
		m_MatrixUB->setData("view", glm::value_ptr(view));

		pbrTextureShader->Bind();
		//pbrTextureShader->SetUniformMat4("view", view);
		pbrTextureShader->SetUniformVec3("camPos", cameraController->GetCamera().GetPosition());

		/*RenderCommand::BindTexture(albedo->GetRendererID(), 0);
		RenderCommand::BindTexture(normal->GetRendererID(), 1);
		RenderCommand::BindTexture(metallic->GetRendererID(), 2);
		RenderCommand::BindTexture(roughness->GetRendererID(), 3);
		RenderCommand::BindTexture(ao->GetRendererID(), 4);*/

		RenderCommand::BindCubemap(Irradiancemap->GetRendererID(), 5);
		RenderCommand::BindCubemap(Prefiltermap->GetRendererID(), 6);
		RenderCommand::BindTexture(brdfLUTTexture->GetRendererID(), 7);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, 0));
		pbrTextureShader->SetUniformMat4("model", model);
		//testModel->Draw(pbrTextureShader);
		//Renderer::DrawModel(testModel, pbrTextureShader);
		//sphere->RenderSphere();
		m_ActiveScene->OnUpdate(ts);

		
		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
		// keeps the codeprint small.
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(RenderCommand::GetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			pbrShader->Bind();
			pbrShader->SetUniformVec3("albedo", glm::vec3(1.0f, 1.0f, 1.0f));
			pbrShader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrShader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			pbrShader->SetUniformMat4("model", model);
			sphere->RenderSphere();

			pbrTextureShader->Bind();
			pbrTextureShader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrTextureShader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
		}
		


		// render skybox (render as last to prevent overdraw)
		backgroundShader->Bind();
		RenderCommand::BindCubemap(Cubemap->GetRendererID(), 0);

		square->RenderSquare();

		Renderer::EndScene();
		renderFramebuffer->Unbind();

		//RenderCommand::Clear();
		//RenderCommand::SetClearColor({ 1.f, 0.f, 0.0f, 1.0f });

		//basicShader->Bind();
		//RenderCommand::BindTexture(renderFramebuffer->GetColorAttachmentRendererID(),0);
		//quad->RenderQuad();

	}

	void Editor::OnImGuiRender()
	{
		imguilayer->MainMenu();

		imguilayer->Dockspace(renderFramebuffer);

		if (imguilayer->MenuEnabled) { imguilayer->Menu(); }

		if (imguilayer->WindowMetricsEnabled) { imguilayer->WindowMetrics(); }

		if (imguilayer->EditorStyleEnabled) { imguilayer->StyleEditor(); }

		HierarchyPanel->OnImGuiRender();


		RenderCommand::SetViewport(imguilayer->CurrentWindowView.x, imguilayer->CurrentWindowView.y);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)imguilayer->CurrentWindowView.x / (float)imguilayer->CurrentWindowView.y, 0.1f, 100.0f);
		m_MatrixUB->setData("projection", glm::value_ptr(projection));


	}

	void Editor::OnEvent(CrashEngine::Event& event)
	{
		cameraController->OnEvent(event);

	}

}