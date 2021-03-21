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

		imguilayer.reset(new ImGuiLayer);

		Height = Application::Get().GetWindow().GetHeight();
		Width = Application::Get().GetWindow().GetWidth();

		cameraController.reset(new CameraController(glm::vec3(0.0f, 0.0f, 3.0f), Width, Height));

		//------------framebuffers--------------------
		FramebufferSpecification spec;
		spec.Height = Height;
		spec.Width = Width;

		deferredframebuffer = Framebuffer::Create(spec, false);
		deferredframebuffer->Bind();
		deferredframebuffer->CreateTexture(0, Color::RGBA);//position
		deferredframebuffer->CreateTexture(1, Color::RGBA);//albedo
		deferredframebuffer->CreateTexture(2, Color::RGBA);//normal
		deferredframebuffer->CreateTexture(3, Color::RGBA);//metallic roughness ao
		deferredframebuffer->InitializeMultipleTextures(4);

		framebuffer = Framebuffer::Create(spec);

		//------------shaders-------------------------
		deferredShader = Shader::Create("basic.vert", "deferred.frag");
		deferredShader->Bind();

		deferredShader->SetUniformInt("position", 0);
		deferredShader->SetUniformInt("albedo", 1);
		deferredShader->SetUniformInt("normal", 2);
		deferredShader->SetUniformInt("MetalRoughAO", 3);

		deferredShader->SetUniformInt("irradianceMap", 4);
		deferredShader->SetUniformInt("prefilterMap", 5);
		deferredShader->SetUniformInt("brdfLUT", 6);
		deferredShader->SetUniformInt("shadowMap", 7);
		deferredShader->SetUniformInt("ssao", 8);

		GBufferShader = Shader::Create("pbr.vert", "gbuffer.frag");
		GBufferShader->Bind();
		GBufferShader->SetUniformInt("albedoMap", 0);
		GBufferShader->SetUniformInt("normalMap", 1);
		GBufferShader->SetUniformInt("metallicMap", 2);
		GBufferShader->SetUniformInt("roughnessMap", 3);
		GBufferShader->SetUniformInt("aoMap", 4);


		forwardShader = Shader::Create("pbr.vert", "pbr.frag");
		forwardShader->Bind();

		forwardShader->SetUniformInt("albedoMap", 0);
		forwardShader->SetUniformInt("normalMap", 1);
		forwardShader->SetUniformInt("metallicMap", 2);
		forwardShader->SetUniformInt("roughnessMap", 3);
		forwardShader->SetUniformInt("aoMap", 4);

		forwardShader->SetUniformInt("irradianceMap", 5);
		forwardShader->SetUniformInt("prefilterMap", 6);
		forwardShader->SetUniformInt("brdfLUT", 7);
		forwardShader->SetUniformInt("shadowMap", 8);

		RenderCommand::SetViewport(Width, Height);

		//-------------------------End of initialize-----------------------------------------
		m_ActiveScene = std::make_shared<Scene>();


		directionalLight.reset(new DirectionalLight);
		directionalLight->camera = &cameraController->GetCamera();
		directionalLight->m_ActiveScene = m_ActiveScene;
		directionalLight->Height = Application::Get().GetWindow().GetHeight();
		directionalLight->Width = Application::Get().GetWindow().GetWidth();
		directionalLight->pbrTextureShader = deferredShader;

		skyLight.reset(new SkyLight);

		m_ActiveScene->SetDefaultShader(GBufferShader);
		m_ActiveScene->SetDepthShader(directionalLight->depthMapShader);

		HierarchyPanel.reset(new SceneHierarchyPanel(m_ActiveScene));
		EnvironmentPanel.reset(new SceneEnvironmentPanel(skyLight, directionalLight, m_ActiveScene));

		SceneSerializer serializer(m_ActiveScene, skyLight, directionalLight);
		serializer.Deserialize("C:/EngineDev/CrashEngine/Models/Scenes/test1.crash");

		//-------------------------End of initialize-----------------------------------------

		glm::mat4 projection = cameraController->GetCamera().GetProjectionMatrix();
		UniformBufferLayout uniformLayout = {
			{ ShaderDataType::Mat4, "projection" },
			{ ShaderDataType::Mat4, "view"},
		};

		m_MatrixUB.reset(UniformBuffer::Create(uniformLayout, 0));
		m_MatrixUB->linkShader(forwardShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(deferredShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(skyLight->GetSkyShader()->GetID(), "Matrices");
		m_MatrixUB->linkShader(GBufferShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(m_ActiveScene->ssao->ssaoShader->GetID(), "Matrices");

		m_MatrixUB->setData("projection", glm::value_ptr(projection));
		
		
	}

	void Editor::OnUpdate(Timestep ts)
	{
		Renderer::BeginScene();
		cameraController->OnUpdate(ts);

		//----------------shadows----------------
		//directionalLight->DrawCSM();
		//----------------shadows----------------

		Height = imguilayer->CurrentWindowView.y;
		Width = imguilayer->CurrentWindowView.x;

		deferredframebuffer->Resize(Width, Height);
		framebuffer->Resize(Width, Height);
		

		glm::mat4 view = cameraController->GetCamera().GetViewMatrix();
		m_MatrixUB->setData("view", glm::value_ptr(view));

		//-----------deffered------------------------

		deferredframebuffer->Bind();
		RenderCommand::SetViewport(Width, Height);
		RenderCommand::SetClearColor({ 1.f, 0.f, 0.0f, 0.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts);
		deferredframebuffer->Unbind();

		//-------ssao-------
		m_ActiveScene->ssao->Render(Width, Height, deferredframebuffer->GetColorAttachmentRendererID(0), deferredframebuffer->GetColorAttachmentRendererID(2));
		//-------ssao-------

		framebuffer->Bind();
		RenderCommand::SetViewport(Width, Height);
		RenderCommand::Clear();

		deferredShader->Bind();

		RenderCommand::BindTexture(deferredframebuffer->GetColorAttachmentRendererID(0), 0);
		RenderCommand::BindTexture(deferredframebuffer->GetColorAttachmentRendererID(1), 1);
		RenderCommand::BindTexture(deferredframebuffer->GetColorAttachmentRendererID(2), 2);
		RenderCommand::BindTexture(deferredframebuffer->GetColorAttachmentRendererID(3), 3);
		RenderCommand::BindTexture(m_ActiveScene->ssao->GetTextureRendererID(), 8);

		skyLight->BindIrradianceMap(4);
		skyLight->BindPrefilterMap(5);
		skyLight->BindbrdfTexture(6);

		glm::vec3 rotation = glm::mat3(view) * glm::vec3(
			glm::cos(directionalLight->rotation.x) * glm::sin(directionalLight->rotation.y),
			glm::sin(directionalLight->rotation.x) * glm::sin(directionalLight->rotation.y),
			glm::cos(directionalLight->rotation.y));
		deferredShader->SetUniformVec3("lightRotation", rotation);
		deferredShader->SetUniformVec3("lightColor", directionalLight->color * directionalLight->intensity);
		deferredShader->SetUniformVec3("camPos", cameraController->GetCamera().GetPosition());
		//RenderCommand::BindTexture(directionalLight->depthMap[0]->GetRendererID(), 7);

		deferredShader->Bind();
		quad->RenderQuad();

		deferredframebuffer->BlitDepthToFramebuffer(framebuffer);

		framebuffer->Bind();
		skyLight->RenderSky();

		framebuffer->Unbind();
		//-----------deffered------------------------


		//-----------Post Proscess-------------------
		m_ActiveScene->postProcess->ApplyFXAA(framebuffer);
		m_ActiveScene->postProcess->Blur(framebuffer);
		m_ActiveScene->postProcess->GammaHDRCorretion(framebuffer);
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
					directionalLight->pbrTextureShader = deferredShader;

					skyLight.reset(new SkyLight);
					m_MatrixUB->linkShader(skyLight->GetSkyShader()->GetID(), "Matrices");

					m_ActiveScene->SetDefaultShader(GBufferShader);
					m_ActiveScene->SetDepthShader(directionalLight->depthMapShader);

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
						directionalLight->pbrTextureShader = deferredShader;

						skyLight.reset(new SkyLight);
						m_MatrixUB->linkShader(skyLight->GetSkyShader()->GetID(), "Matrices");

						m_ActiveScene->SetDefaultShader(GBufferShader);
						m_ActiveScene->SetDepthShader(directionalLight->depthMapShader);

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
		ImGui::SliderFloat("Camera Speed", &cameraController->m_CameraSpeed, 1.f, 100.f);
		ImGui::Checkbox("ssao", &forward);

		ImGui::Checkbox("metrics", &metrics);
		if (metrics)
		{
			imguilayer->WindowMetrics();

		}
		ImGui::SliderInt("deferred", &deferred, 0, 4);

		if(deferred == 4)ImGui::Image((void*)m_ActiveScene->ssao->GetTextureRendererID(), ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
		else ImGui::Image((void*)deferredframebuffer->GetColorAttachmentRendererID(deferred), ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
		

		ImGui::End();


		glm::mat4 projection = glm::perspective(glm::radians(cameraController->GetCamera().fov), (float)imguilayer->CurrentWindowView.x / (float)imguilayer->CurrentWindowView.y, 0.1f, 400.0f);
		m_MatrixUB->setData("projection", glm::value_ptr(projection));
		

	}

	void Editor::OnEvent(CrashEngine::Event& event)
	{
		cameraController->OnEvent(event);

	}

}