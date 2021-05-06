#include "EditorLayer.h"
#include "CrashEngine/Scene/SceneSerializer.h"
#include "CrashEngine/Utils/PlatformUtils.h"

#include <math.h>
#include <cfloat>

#include "ImGuizmo.h"
#include "CrashEngine/Math/Math.h"


namespace CrashEngine {

	Editor::Editor()
		: Layer("Example")
	{
		// initialize default shapes
		sphere.reset(new Sphere());
		cube.reset(new Cube());
		quad.reset(new Quad());

		//debugLine.reset(new DebugLine());

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
		deferredShader->SetUniformInt("ssao", 7);
		deferredShader->SetUniformInt("shadowMap[0]", 8);
		deferredShader->SetUniformInt("shadowMap[1]", 9);
		deferredShader->SetUniformInt("shadowMap[2]", 10);

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

		HierarchyPanel.reset(new SceneHierarchyPanel(m_ActiveScene));
		EnvironmentPanel.reset(new SceneEnvironmentPanel(skyLight, directionalLight, m_ActiveScene));

		SceneSerializer serializer(m_ActiveScene, skyLight, directionalLight);
		serializer.Deserialize("C:/EngineDev/CrashEngine/Models/Scenes/jd.crash");

		//-------------------------End of initialize-----------------------------------------
		
		Application::Get().GetDebugger().Begin();

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
		m_MatrixUB->linkShader(Application::Get().GetDebugger().GetShader().GetID(), "Matrices");

		m_MatrixUB->setData("projection", glm::value_ptr(projection));

		//---------------------Test space--------------------------------------------------------
		RenderCommand::InitDebugOutput();


		TextureSpecification tspec;
		tspec.Width = 1080;
		tspec.Height = 1080;
		tspec.type = Type::FLOAT;
		tspec.DataFormat = DataFormat::RGBA;
		tspec.FilterParam = FilterParam::LINEAR;
		tspec.internalFormat = InternalFormat::RGBA32F;
		testTexture = Texture2D::Create(tspec);

		debugFramebuffer = Framebuffer::Create(spec);
		debugFramebuffer->Bind();
		debugFramebuffer->Resize(1080, 1080);
		//debugFramebuffer->SetTexture(CE_TEXTURE_2D, testTexture->GetRendererID(), 0);

		compShader = ComputeShader::Create("computeTest.comp");

		QuadShader = Shader::Create("Basic.vert", "TexBasic.frag");
		QuadShader->Bind();
		QuadShader->SetUniformInt("tex", 0);
		
	}

	void Editor::OnUpdate(Timestep ts)
	{
		/*auto sview = m_ActiveScene->m_Registry.view<TransformComponent, Mesh, TagComponent>();

		for (auto entity : sview)
		{
			if (sview.get<TagComponent>(entity).Tag == "Cube1")
			{
				auto& mod = sview.get<Mesh>(entity);
				auto& transform = sview.get<TransformComponent>(entity).Translation = glm::vec3(
					glm::cos(directionalLight->rotation.x) * glm::sin(directionalLight->rotation.y),
					glm::sin(directionalLight->rotation.x) * glm::sin(directionalLight->rotation.y),
					glm::cos(directionalLight->rotation.y));
			}
		}*/

		Renderer::BeginScene();
		cameraController->OnUpdate(ts);

		//----------------shadows----------------
		directionalLight->DrawCSM();
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
		RenderCommand::BindTexture(m_ActiveScene->ssao->GetTextureRendererID(), 7);

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
		for (int i = 0; i < 3; i++)
			directionalLight->depthMap[i]->Bind(8 + i);


		deferredShader->Bind();
		quad->RenderQuad();

		deferredframebuffer->BlitDepthToFramebuffer(framebuffer);
		//-----------deffered------------------------

		//-----------Post Proscess-------------------
		m_ActiveScene->postProcess->Blur(framebuffer);

		framebuffer->Bind();
		skyLight->RenderSky();
		framebuffer->Unbind();

		//-----------Post Proscess-------------------
		m_ActiveScene->postProcess->ApplyFXAA(framebuffer);
		m_ActiveScene->postProcess->GammaHDRCorretion(framebuffer);

		//-----------Debug lines---------------------
		framebuffer->Bind();
		auto& camera = cameraController->GetCamera();
		Application::Get().GetDebugger().OnUpdate(camera);
		framebuffer->Unbind();

		//-----------Test render----------------------
		compShader->Bind();

		testTexture->BindImageTexture();

		RenderCommand::Dispatch(testTexture->GetWidth(),testTexture->GetHeight());
		RenderCommand::MemoryBarier();


		debugFramebuffer->Bind();
		RenderCommand::SetViewport(1080, 1080);
		QuadShader->Bind();
		RenderCommand::BindTexture(testTexture->GetRendererID(), 0);
		RenderCommand::Clear();
		quad->RenderQuad();
		debugFramebuffer->Unbind();

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

						HierarchyPanel.reset(new SceneHierarchyPanel(m_ActiveScene));
						EnvironmentPanel.reset(new SceneEnvironmentPanel(skyLight, directionalLight, m_ActiveScene));

						SceneSerializer serializer(m_ActiveScene, skyLight, directionalLight);
						serializer.Deserialize(filepath.value());
					}
				}

				if (ImGui::MenuItem("Save")) 
				{
					std::optional<std::string> filepath = m_ActiveScene->filepath;
					if (filepath)
					{
						SceneSerializer serializer(m_ActiveScene, skyLight, directionalLight);
						serializer.Serialize(filepath.value());
					}
					else
					{
						std::optional<std::string> filepathAs = FileDialogs::SaveFile("(*.crash)\0*.crash\0");
						if (filepathAs)
						{
							m_ActiveScene->filepath = filepathAs.value();
							SceneSerializer serializer(m_ActiveScene, skyLight, directionalLight);
							serializer.Serialize(filepathAs.value());
						}
					}
				}
				if (ImGui::MenuItem("Save As.."))
				{
					std::optional<std::string> filepath = FileDialogs::SaveFile("(*.crash)\0*.crash\0");
					if (filepath)
					{
						m_ActiveScene->filepath = filepath.value();
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

		//Gizmos
		auto& camera = cameraController->GetCamera();
		const glm::mat4& cameraProjection = camera.GetProjectionMatrix();
		glm::mat4 cameraView = camera.GetViewMatrix();

		glm::mat4 identityMatrix = glm::mat4(1.f);

		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::AllowAxisFlip(false);		
		
		//ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(identityMatrix), 100.f);

		Entity selectedEntity = HierarchyPanel->GetSelectedEntity();
		if (selectedEntity && gizmoType != -1)
		{
			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(CE_KEY_LEFT_CONTROL);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (gizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}
		
		ImGui::End();
		ImGui::End();


		if (imguilayer->MenuEnabled) { imguilayer->Menu(); }
		if (imguilayer->WindowMetricsEnabled) { imguilayer->WindowMetrics(); }
		if (imguilayer->EditorStyleEnabled) { imguilayer->StyleEditor(); }


		HierarchyPanel->OnImGuiRender();
		EnvironmentPanel->OnImGuiRender();





		ImGui::Begin("Debug");
		if (ImGui::SliderInt("samples of blur", &skyLight->BlurSamples, 0, 50))
		{
			skyLight->UpdateCubemap();
		}
		
		if (ImGui::Button("Show Info"))
		{

		}
		ImGui::SliderFloat("Camera Speed", &cameraController->m_CameraSpeed, 1.f, 100.f);
		ImGui::Checkbox("ssao", &forward);

		
		ImGui::Checkbox("metrics", &metrics);
		if (metrics)
		{
			imguilayer->WindowMetrics();

	}
		

		ImGui::SliderInt("shadow", &deferred, 0, 2);
		ImGui::Image((void*)directionalLight->depthMap[deferred]->GetRendererID(), ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::Image((void*)debugFramebuffer->GetColorAttachmentRendererID(), ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::SliderFloat("far_plane", &directionalLight->far_plane, -10, 200);
		ImGui::SliderFloat("near_plane", &directionalLight->near_plane, -200, -10);
		ImGui::SliderFloat("size", &directionalLight->size, 0, 200);

		ImGui::Checkbox("Use colors CSM", &csm);

		deferredShader->Bind();
		deferredShader->SetUniformInt("csmColor", csm);

		ImGui::End();
		
		glm::mat4 projection = glm::perspective(glm::radians(cameraController->GetCamera().fov), (float)imguilayer->CurrentWindowView.x / (float)imguilayer->CurrentWindowView.y, 0.1f, 1500.0f);
		m_MatrixUB->setData("projection", glm::value_ptr(projection));
		cameraController->GetCamera().SetProjection(projection);
	}

	void Editor::OnEvent(CrashEngine::Event& event)
	{
		cameraController->OnEvent(event);

		//EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<KeyPressedEvent>();

		if (event.GetEventType() == EventType::KeyPressed)
		{
			CrashEngine::KeyPressedEvent& e = (CrashEngine::KeyPressedEvent&)event;

			switch (e.GetKeyCode())
			{
			case CE_KEY_1:
				gizmoType = -1;
				break;
			case CE_KEY_2:
				gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case CE_KEY_3:
				gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case CE_KEY_4:
				gizmoType = ImGuizmo::OPERATION::SCALE;
				break;

			}
		}



	}

}