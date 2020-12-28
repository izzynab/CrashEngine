#include "EditorLayer.h"

namespace CrashEngine {

	Editor::Editor()
		: Layer("Example")
	{
		//auto window = Application::Get().GetWindow();

		// initialize default shapes
		sphere.reset(new Sphere());
		cube.reset(new Cube());
		quad.reset(new Quad());

		float Height = Application::Get().GetWindow().GetHeight();
		float Width = Application::Get().GetWindow().GetWidth();

		cameraController.reset(new CameraController(glm::vec3(0.0f, 0.0f, 3.0f), Width, Height));

		FramebufferSpecification spec;
		spec.Height = 1080;
		spec.Width = 2560;

		Framebuffer = Framebuffer::Create(spec);

		imguilayer.reset(new ImGuiLayer);

		glm::mat4 projection = cameraController->GetCamera().GetProjectionMatrix();

		basicShader = Shader::Create("Basic.vert", "Basic.frag");


		pbrShader = Shader::Create("pbr.vert", "pbr.frag");
		pbrShader->Bind();

		pbrShader->SetUniformVec3("albedo", glm::vec3(0.3f, 0.3f, 0.3f));
		pbrShader->SetUniformFloat("ao", 1.0f);
		pbrShader->SetUniformInt("irradianceMap", 0);
		pbrShader->SetUniformInt("prefilterMap", 1);
		pbrShader->SetUniformInt("brdfLUT", 2);


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


		skyLight.reset(new SkyLight);

		UniformBufferLayout uniformLayout = {
			{ ShaderDataType::Mat4, "projection" },
			{ ShaderDataType::Mat4, "view"},
		};

		m_MatrixUB.reset(UniformBuffer::Create(uniformLayout, 0));
		m_MatrixUB->linkShader(pbrShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(pbrTextureShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(skyLight->GetSkyShader()->GetID(), "Matrices");

		m_MatrixUB->setData("projection", glm::value_ptr(projection));


		RenderCommand::SetViewport(Width, Height);

	//-------------------------End of initialize-----------------------------------------


		//Scene and entities----------------------------------
		m_ActiveScene = std::make_shared<Scene>();


		auto mesh1 = m_ActiveScene->CreateEntity("Gun");

		MeshComponent mesh = MeshComponent("C:\\EngineDev\\CrashEngine\\Models\\cerberus\\cerberus.obj");

		/*mesh.albedo = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_A.tga");
		mesh.normal = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_N.tga");
		mesh.metallic = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_M.tga");
		mesh.roughness = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_R.tga");
		mesh.ao = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_AO.tga");*/

		mesh1.AddComponent<MeshComponent>(mesh);

		m_ActiveScene->SetDefaultShader(pbrTextureShader);

		HierarchyPanel.reset(new SceneHierarchyPanel(m_ActiveScene));
		EnvironmentPanel.reset(new SceneEnvironmentPanel(skyLight));

	}

	void Editor::OnUpdate(Timestep ts)
	{
		cameraController->OnUpdate(ts);

		Framebuffer->Bind();

		RenderCommand::Clear();
		RenderCommand::SetClearColor({ 1.f, 0.f, 0.0f, 1.0f });

		Renderer::BeginScene();

		glm::mat4 view = cameraController->GetCamera().GetViewMatrix();
		m_MatrixUB->setData("view", glm::value_ptr(view));

		pbrTextureShader->Bind();
		//pbrTextureShader->SetUniformMat4("view", view);
		pbrTextureShader->SetUniformVec3("camPos", cameraController->GetCamera().GetPosition());

		skyLight->BindIrradianceMap(5);
		skyLight->BindPrefilterMap(6);
		skyLight->BindbrdfTexture(7);

		m_ActiveScene->OnUpdate(ts);


		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(RenderCommand::GetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];

			pbrTextureShader->Bind();
			pbrTextureShader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrTextureShader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
		}

		// render skybox (render as last to prevent overdraw)
		skyLight->RenderSky();

		Renderer::EndScene();
		Framebuffer->Unbind();


	}

	void Editor::OnImGuiRender()
	{
		imguilayer->MainMenu();
		imguilayer->Dockspace(Framebuffer);

		if (imguilayer->MenuEnabled) { imguilayer->Menu(); }

		if (imguilayer->WindowMetricsEnabled) { imguilayer->WindowMetrics(); }

		if (imguilayer->EditorStyleEnabled) { imguilayer->StyleEditor(); }

		HierarchyPanel->OnImGuiRender();
		EnvironmentPanel->OnImGuiRender();

	
		RenderCommand::SetViewport(imguilayer->CurrentWindowView.x, imguilayer->CurrentWindowView.y);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)imguilayer->CurrentWindowView.x / (float)imguilayer->CurrentWindowView.y, 0.1f, 100.0f);
		m_MatrixUB->setData("projection", glm::value_ptr(projection));
		

	}

	void Editor::OnEvent(CrashEngine::Event& event)
	{
		cameraController->OnEvent(event);

	}

}