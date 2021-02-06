#include "EditorLayer.h"
#include "CrashEngine/Scene/SceneSerializer.h"
#include "CrashEngine/Utils/PlatformUtils.h"

#include <math.h>
#include <cfloat>

namespace CrashEngine {

	Editor::Editor()
		: Layer("Example")
	{
		// initialize default shapes
		sphere.reset(new Sphere());
		cube.reset(new Cube());
		quad.reset(new Quad());

		Height = Application::Get().GetWindow().GetHeight();
		Width = Application::Get().GetWindow().GetWidth();

		cameraController.reset(new CameraController(glm::vec3(0.0f, 0.0f, 3.0f), Width, Height));

		FramebufferSpecification spec;
		spec.Height = 1080;
		spec.Width = 2560;

		MSAAframebuffer = Framebuffer::Create(spec,false);
		MSAAframebuffer->CreateMSAATexture();

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


		RenderCommand::SetViewport(Width, Height);

		//-------------------------End of initialize-----------------------------------------
		m_ActiveScene = std::make_shared<Scene>();

		directionalLight.reset(new DirectionalLight);
		directionalLight->camera = &cameraController->GetCamera();
		directionalLight->m_ActiveScene = m_ActiveScene;
		directionalLight->Height = Application::Get().GetWindow().GetHeight();
		directionalLight->Width = Application::Get().GetWindow().GetWidth();
		directionalLight->pbrTextureShader = pbrTextureShader;

		skyLight.reset(new SkyLight);

		m_ActiveScene->SetDefaultShader(pbrTextureShader);
		m_ActiveScene->SetDepthShader(directionalLight->depthMapShader);
		m_ActiveScene->SetFramebuffers(MSAAframebuffer,framebuffer);

		HierarchyPanel.reset(new SceneHierarchyPanel(m_ActiveScene));
		EnvironmentPanel.reset(new SceneEnvironmentPanel(skyLight, directionalLight, m_ActiveScene));

		SceneSerializer serializer(m_ActiveScene, skyLight, directionalLight);
		serializer.Deserialize("C:/EngineDev/CrashEngine/Models/Scenes/test3.crash");

		//-------------------------End of initialize-----------------------------------------

		UniformBufferLayout uniformLayout = {
			{ ShaderDataType::Mat4, "projection" },
			{ ShaderDataType::Mat4, "view"},
		};

		m_MatrixUB.reset(UniformBuffer::Create(uniformLayout, 0));
		m_MatrixUB->linkShader(pbrTextureShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(skyLight->GetSkyShader()->GetID(), "Matrices");

		m_MatrixUB->setData("projection", glm::value_ptr(projection));


	}

	void Editor::OnUpdate(Timestep ts)
	{
		Renderer::BeginScene();
		cameraController->OnUpdate(ts);

		//----------------shadows----------------
		//directionalLight->DrawCSM();
		//----------------shadows----------------


		MSAAframebuffer->Bind();
		RenderCommand::SetViewport(imguilayer->CurrentWindowView.x, imguilayer->CurrentWindowView.y);
		MSAAframebuffer->Resize(imguilayer->CurrentWindowView.x, imguilayer->CurrentWindowView.y);
		RenderCommand::SetClearColor({ 1.f, 0.f, 0.0f, 0.0f });
		RenderCommand::Clear();

		glm::mat4 view = cameraController->GetCamera().GetViewMatrix();
		m_MatrixUB->setData("view", glm::value_ptr(view));
		
		pbrTextureShader->Bind();
		pbrTextureShader->SetUniformVec3("camPos", cameraController->GetCamera().GetPosition());

		skyLight->BindIrradianceMap(5);
		skyLight->BindPrefilterMap(6);
		skyLight->BindbrdfTexture(7);

		pbrTextureShader->Bind();
		pbrTextureShader->SetUniformVec3("lightRotation", directionalLight->rotation);
		pbrTextureShader->SetUniformVec3("lightColor", directionalLight->color * directionalLight->intensity);
		//RenderCommand::BindTexture(directionalLight->depthMap[0]->GetRendererID(), 8);

		m_ActiveScene->OnUpdate(ts);

		skyLight->RenderSky();	

		m_ActiveScene->BlurRender();


		Renderer::EndScene();


	}

	void Editor::OnImGuiRender()
	{
		//imguilayer->MainMenu();
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Hello, it's my menu", NULL, false, false);
				if (ImGui::MenuItem("New"))
				{
					m_ActiveScene = std::make_shared<Scene>();

					directionalLight.reset(new DirectionalLight);
					directionalLight->camera = &cameraController->GetCamera();
					directionalLight->m_ActiveScene = m_ActiveScene;
					directionalLight->Height = Application::Get().GetWindow().GetHeight();
					directionalLight->Width = Application::Get().GetWindow().GetWidth();
					directionalLight->pbrTextureShader = pbrTextureShader;

					skyLight.reset(new SkyLight);

					m_ActiveScene->SetDefaultShader(pbrTextureShader);
					m_ActiveScene->SetDepthShader(directionalLight->depthMapShader);
					m_ActiveScene->SetFramebuffers(MSAAframebuffer, framebuffer);

					HierarchyPanel.reset(new SceneHierarchyPanel(m_ActiveScene));
					EnvironmentPanel.reset(new SceneEnvironmentPanel(skyLight, directionalLight, m_ActiveScene));
				}
				if (ImGui::MenuItem("Open"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("(*.crash)\0*.crash\0");
					if (filepath)
					{
						m_ActiveScene = std::make_shared<Scene>();

						directionalLight.reset(new DirectionalLight);
						directionalLight->camera = &cameraController->GetCamera();
						directionalLight->m_ActiveScene = m_ActiveScene;
						directionalLight->Height = Application::Get().GetWindow().GetHeight();
						directionalLight->Width = Application::Get().GetWindow().GetWidth();
						directionalLight->pbrTextureShader = pbrTextureShader;

						skyLight.reset(new SkyLight);

						m_ActiveScene->SetDefaultShader(pbrTextureShader);
						m_ActiveScene->SetDepthShader(directionalLight->depthMapShader);
						m_ActiveScene->SetFramebuffers(MSAAframebuffer, framebuffer);

						HierarchyPanel.reset(new SceneHierarchyPanel(m_ActiveScene));
						EnvironmentPanel.reset(new SceneEnvironmentPanel(skyLight, directionalLight, m_ActiveScene));

						SceneSerializer serializer(m_ActiveScene, skyLight, directionalLight);
						serializer.Deserialize(filepath.value());
					}
				}

				if (ImGui::MenuItem("Save")) {}
				if (ImGui::MenuItem("Save As.."))
				{
					std::optional<std::string> filepath = FileDialogs::SaveFile("(*.crash)\0*.crash\0");
					if (filepath)
					{
						SceneSerializer serializer(m_ActiveScene, skyLight, directionalLight);
						serializer.Serialize(filepath.value());
					}
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Quit")) {}


				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		imguilayer->Dockspace(framebuffer);

		if (imguilayer->MenuEnabled) { imguilayer->Menu(); }

		if (imguilayer->WindowMetricsEnabled) { imguilayer->WindowMetrics(); }

		if (imguilayer->EditorStyleEnabled) { imguilayer->StyleEditor(); }

		HierarchyPanel->OnImGuiRender();
		EnvironmentPanel->OnImGuiRender();
	
		ImGui::Begin("Debug");
		ImGui::SliderFloat("Camera Speed", &cameraController->m_CameraSpeed, 1.f, 40.f);
		//ImGui::SliderInt("Cascade map", &cascademapselected, 1, 3);
		//ImGui::Image((void*)directionalLight->depthMap[cascademapselected-1]->GetRendererID(), ImVec2(400,400));
		ImGui::Image((void*)framebuffer->GetColorAttachmentRendererID(0), ImVec2(400, 400));
		ImGui::End();

	
		//RenderCommand::SetViewport(imguilayer->CurrentWindowView.x, imguilayer->CurrentWindowView.y);

		glm::mat4 projection = glm::perspective(glm::radians(cameraController->GetCamera().fov), (float)imguilayer->CurrentWindowView.x / (float)imguilayer->CurrentWindowView.y, 0.1f, 400.0f);
		m_MatrixUB->setData("projection", glm::value_ptr(projection));
		

	}

	void Editor::OnEvent(CrashEngine::Event& event)
	{
		cameraController->OnEvent(event);

	}

}