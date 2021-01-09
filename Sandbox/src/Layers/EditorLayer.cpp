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

		framebuffer = Framebuffer::Create(spec);

		imguilayer.reset(new ImGuiLayer);

		glm::mat4 projection = cameraController->GetCamera().GetProjectionMatrix();

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
		pbrTextureShader->SetUniformInt("shadowMap", 8);




		depthMapShader = Shader::Create("depthMap.vert", "depthMap.frag");

		FramebufferSpecification depthspec;
		spec.Height = 1024;
		spec.Width = 1024;

		depthFramebuffer = Framebuffer::Create(depthspec,false);

		depthMap = DepthTexture::Create(1024, 1024);
		depthFramebuffer->Bind();
		depthFramebuffer->SetDepthTexture(CE_TEXTURE_2D, depthMap->GetRendererID());
		depthFramebuffer->Unbind();


		




		skyLight.reset(new SkyLight);
		skyLight->LoadHDR("C:\\EngineDev\\CrashEngine\\Textures\\hdr/14-Hamarikyu_Bridge_B_3k.hdr");

		directionalLight.reset(new DirectionalLight);

		UniformBufferLayout uniformLayout = {
			{ ShaderDataType::Mat4, "projection" },
			{ ShaderDataType::Mat4, "view"},
		};

		m_MatrixUB.reset(UniformBuffer::Create(uniformLayout, 0));
		m_MatrixUB->linkShader(pbrTextureShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(skyLight->GetSkyShader()->GetID(), "Matrices");

		m_MatrixUB->setData("projection", glm::value_ptr(projection));


		RenderCommand::SetViewport(Width, Height);

	//-------------------------End of initialize-----------------------------------------


		//Scene and entities----------------------------------
		m_ActiveScene = std::make_shared<Scene>();


		auto mesh1 = m_ActiveScene->CreateEntity("Cube 1");
		auto mesh2 = m_ActiveScene->CreateEntity("Cube 2");
		auto mesh3 = m_ActiveScene->CreateEntity("Cube 3");
		auto mesh4 = m_ActiveScene->CreateEntity("Cube 4");//todo: last mesh added to scene isnt visible to depth map

		Mesh mesh10 = Mesh("C:\\EngineDev\\CrashEngine\\Models\\cube.obj");
		Mesh mesh20 = Mesh("C:\\EngineDev\\CrashEngine\\Models\\cube.obj");
		Mesh mesh30 = Mesh("C:\\EngineDev\\CrashEngine\\Models\\cube.obj");
		Mesh mesh40 = Mesh("C:\\EngineDev\\CrashEngine\\Models\\sphere.obj");
		mesh10.material->name.reset(new std::string("material for fisrt mesh"));

		mesh1.AddComponent<Mesh>(mesh10);
		mesh1.GetComponent<TransformComponent>().Translation = glm::vec3(-4, -2, 0);
		mesh1.GetComponent<TransformComponent>().Scale = glm::vec3(10, 0.1, 14);

		mesh2.AddComponent<Mesh>(mesh20);
		mesh2.GetComponent<TransformComponent>().Translation = glm::vec3(-7, 0, 0);
		mesh2.GetComponent<TransformComponent>().Scale = glm::vec3(1, 2.5, 5);

		mesh3.AddComponent<Mesh>(mesh30);
		mesh3.GetComponent<TransformComponent>().Translation = glm::vec3(0, 3, 3);
		mesh3.GetComponent<TransformComponent>().Scale = glm::vec3(1, 3, 4);
		mesh3.GetComponent<TransformComponent>().Rotation = glm::vec3(0, 0.2, 0.44);

		mesh4.AddComponent<Mesh>(mesh40);
		mesh4.GetComponent<TransformComponent>().Translation = glm::vec3(-6, 2, 6);
		mesh4.GetComponent<TransformComponent>().Scale = glm::vec3(1, 1, 1);

		m_ActiveScene->SetDefaultShader(pbrTextureShader);
		m_ActiveScene->SetDepthShader(depthMapShader);

		HierarchyPanel.reset(new SceneHierarchyPanel(m_ActiveScene));
		EnvironmentPanel.reset(new SceneEnvironmentPanel(skyLight, directionalLight));

	}

	void Editor::OnUpdate(Timestep ts)
	{
		Renderer::BeginScene();
		cameraController->OnUpdate(ts);

		//----------------shadows----------------
		//directionalLight->position = cameraController->GetCamera().GetPosition() + glm::vec3(0, 20, 0);
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 100.5f;
		lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
		lightView = glm::lookAt(directionalLight->position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		depthMapShader->Bind();
		depthMapShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
		pbrTextureShader->Bind();
		pbrTextureShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

		RenderCommand::SetViewport(1024, 1024);
		depthFramebuffer->Bind();	
		RenderCommand::Clear(); 
		m_ActiveScene->DepthRender();
		depthFramebuffer->Unbind();

		
		//----------------shadows----------------

		RenderCommand::SetViewport(imguilayer->CurrentWindowView.x, imguilayer->CurrentWindowView.y);
		framebuffer->Bind();
		RenderCommand::SetClearColor({ 1.f, 0.f, 0.0f, 1.0f });
		RenderCommand::Clear();

		glm::mat4 view = cameraController->GetCamera().GetViewMatrix();
		m_MatrixUB->setData("view", glm::value_ptr(view));

		pbrTextureShader->Bind();
		pbrTextureShader->SetUniformVec3("camPos", cameraController->GetCamera().GetPosition());

		skyLight->BindIrradianceMap(5);
		skyLight->BindPrefilterMap(6);
		skyLight->BindbrdfTexture(7);

		pbrTextureShader->Bind();
		pbrTextureShader->SetUniformVec3("lightPosition", directionalLight->position);
		pbrTextureShader->SetUniformVec3("lightColor", directionalLight->color * directionalLight->intensity);
		RenderCommand::BindTexture(depthMap->GetRendererID(), 8);

		m_ActiveScene->OnUpdate(ts);


		skyLight->RenderSky();
		
		

		framebuffer->Unbind();
		Renderer::EndScene();


	}

	void Editor::OnImGuiRender()
	{
		imguilayer->MainMenu();
		imguilayer->Dockspace(framebuffer);

		if (imguilayer->MenuEnabled) { imguilayer->Menu(); }

		if (imguilayer->WindowMetricsEnabled) { imguilayer->WindowMetrics(); }

		if (imguilayer->EditorStyleEnabled) { imguilayer->StyleEditor(); }

		HierarchyPanel->OnImGuiRender();
		EnvironmentPanel->OnImGuiRender();
	
		ImGui::Begin("Camera controller");
		ImGui::SliderFloat("Camera Speed", &cameraController->m_CameraSpeed, 1.f, 40.f);
		ImGui::Image((void*)depthMap->GetRendererID(), ImVec2(400,400));
		ImGui::End();

	
		//RenderCommand::SetViewport(imguilayer->CurrentWindowView.x, imguilayer->CurrentWindowView.y);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)imguilayer->CurrentWindowView.x / (float)imguilayer->CurrentWindowView.y, 0.1f, 100.0f);
		m_MatrixUB->setData("projection", glm::value_ptr(projection));
		

	}

	void Editor::OnEvent(CrashEngine::Event& event)
	{
		cameraController->OnEvent(event);

	}

}