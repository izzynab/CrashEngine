#include "EditorLayer.h"
#include "CrashEngine/Scene/SceneSerializer.h"
#include "CrashEngine/Utils/PlatformUtils.h"

#include <math.h>
#include <cfloat>

#include "ImGuizmo.h"
#include "CrashEngine/Math/Math.h"

//#include "ImGuiFileDialog/ImGuiFileDialog.h"


namespace CrashEngine {

	Editor::Editor()
		: Layer("Example")
	{
		// initialize default shapes
		m_Sphere.reset(new Sphere());
		m_Cube.reset(new Cube());
		m_Quad.reset(new Quad());

		//------------framebuffers--------------------

		FramebufferSpecification spec;
		spec.Height = Application::Get().GetWindow().GetHeight();
		spec.Width = Application::Get().GetWindow().GetWidth();

		m_RenderProperties.reset(new RenderProperties());

		Renderer::AddView(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), "Main", m_RenderProperties, framebuffers);

		editorCameraController = m_RenderProperties->GetCameraController(0);


		m_HierarchyPanel.reset(new SceneHierarchyPanel(m_RenderProperties->GetScene()));
		m_EnvironmentPanel.reset(new SceneEnvironmentPanel(m_RenderProperties->GetScene()));
		m_ContentBrowserPanel.reset(new ContentBrowserPanel());

		SceneSerializer serializer(m_RenderProperties->GetScene());
		serializer.Deserialize("C:/EngineDev/CrashEngine/Content/Scenes/test.crash");


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

		Application::Get().GetDebugger().OnFirstFrame();
	}

	void Editor::OnUpdate(Timestep ts)
	{
		Renderer::BeginScene();

		auto cameraview = m_RenderProperties->GetScene()->m_Registry.view<TransformComponent, CameraComponent>();
		//todo: make better camera transform managing | distinguish between world and self rotation | learn how spherical coordinates work
		for (auto entity : cameraview)
		{
			if (cameraview.get<CameraComponent>(entity).DrawFrustum == true)
			{
				Application::Get().GetDebugger().DrawFrustum(cameraview.get<CameraComponent>(entity).camera.get());
			}

			auto& camera = cameraview.get<CameraComponent>(entity);
			auto& transform = cameraview.get<TransformComponent>(entity);

			camera.camera->SetPosition(transform.Translation);

			glm::vec3 direction;
			direction.x = cos(-transform.Rotation.y) * cos(transform.Rotation.z);
			direction.y = sin(transform.Rotation.z);
			direction.z = sin(-transform.Rotation.y) * cos(transform.Rotation.z);
			//todo: fix scene camera rotation
			camera.camera->SetRotation(glm::normalize(direction));

		}

		//Application::Get().GetDebugger().DrawFrustum(m_RenderProperties->GetCamera(0).get());
		//Application::Get().GetDebugger().DrawUpdateLine(glm::vec3(0), glm::vec3(10));

	
		if (!viewName.empty())editorCameraController->OnUpdate(ts);
		Renderer::RenderScene(m_RenderProperties, framebuffers,ts);
		
		Renderer::EndScene();

		//-----------Test render----------------------
		compShader->Bind();
		testTexture->BindImageTexture();
		RenderCommand::Dispatch(testTexture->GetWidth(),testTexture->GetHeight());
		RenderCommand::MemoryBarier();

		/*debugFramebuffer->Bind();
		RenderCommand::SetViewport(1080, 1080);
		QuadShader->Bind();
		RenderCommand::BindTexture(testTexture->GetRendererID(), 0);
		RenderCommand::Clear();
		quad->RenderQuad();
		debugFramebuffer->Unbind();*/
	}

	void Editor::OnImGuiRender()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Hello, it's my menu", NULL, false, false);
				if (ImGui::MenuItem("New"))
				{
					m_RenderProperties.reset(new RenderProperties());

					Renderer::AddView(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), "Main", m_RenderProperties, framebuffers);

					editorCameraController = m_RenderProperties->GetCameraController(0);

					m_HierarchyPanel.reset(new SceneHierarchyPanel(m_RenderProperties->GetScene()));
					m_EnvironmentPanel.reset(new SceneEnvironmentPanel(m_RenderProperties->GetScene()));
				}
				if (ImGui::MenuItem("Open"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("(*.crash)\0*.crash\0");
					if (filepath)
					{
						m_RenderProperties.reset(new RenderProperties());

						Renderer::AddView(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), "Main", m_RenderProperties, framebuffers);

						editorCameraController = m_RenderProperties->GetCameraController(0);

						m_HierarchyPanel.reset(new SceneHierarchyPanel(m_RenderProperties->GetScene()));
						m_EnvironmentPanel.reset(new SceneEnvironmentPanel(m_RenderProperties->GetScene()));

						SceneSerializer serializer(m_RenderProperties->GetScene());
						serializer.Deserialize(filepath.value());
					}
				}

				if (ImGui::MenuItem("Save")) 
				{
					std::optional<std::string> filepath = m_RenderProperties->GetScene()->filepath;
					if (filepath > " ")
					{
						SceneSerializer serializer(m_RenderProperties->GetScene());
						serializer.Serialize(filepath.value());
					}
					else
					{
						std::optional<std::string> filepathAs = FileDialogs::SaveFile("(*.crash)\0*.crash\0");
						if (filepathAs)
						{
							m_RenderProperties->GetScene()->filepath = filepathAs.value();
							SceneSerializer serializer(m_RenderProperties->GetScene());
							serializer.Serialize(filepathAs.value());
						}
					}
				}
				if (ImGui::MenuItem("Save As.."))
				{
					std::optional<std::string> filepath = FileDialogs::SaveFile("(*.crash)\0*.crash\0");
					if (filepath)
					{
						m_RenderProperties->GetScene()->filepath = filepath.value();
						SceneSerializer serializer(m_RenderProperties->GetScene());
						serializer.Serialize(filepath.value());
					}
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Quit")) {}


				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools"))
			{
				if (ImGui::MenuItem("Add view")) 
				{
					if(!m_RenderProperties->views.empty())Renderer::AddView(800, 800, "View " + std::to_string(m_RenderProperties->views.back().id), m_RenderProperties, framebuffers);
					else Renderer::AddView(800, 800, "Main", m_RenderProperties, framebuffers);
				}

				ImGui::MenuItem("Window metrices", NULL, &WindowMetricsEnabled);

				ImGui::EndMenu();		
			}
			ImGui::EndMainMenuBar();
		}


		bool s = true;
		bool* show = &s;
	
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace", show, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);
		
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		if (!isCursorVisible)
		{
			ImGui::SetWindowFocus(viewName.c_str());
		}
		else viewName.clear();

		for (int i = 0; i < m_RenderProperties->GetViewsNumber(); i++)
		{
			if (!m_RenderProperties->views[i].isOpen)
			{
				m_RenderProperties->views.erase(m_RenderProperties->views.begin() + i);
				i--;
				continue;
			}

			ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;	
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin(m_RenderProperties->GetViewName(i).c_str(), &m_RenderProperties->views[i].isOpen, flags);
			ImGui::PopStyleVar();

			ImVec2 CurrentWindowView = ImVec2(ImGui::GetWindowContentRegionMax().x, ImGui::GetWindowContentRegionMax().y);
			m_RenderProperties->GetCamera(i)->SetSize(CurrentWindowView.y, CurrentWindowView.x);

			ImVec2 imageSize = ImVec2(framebuffers[i]->GetSpecification().Width, framebuffers[i]->GetSpecification().Height);

			ImGui::Image((void*)framebuffers[i]->GetColorAttachmentRendererID(0), imageSize, ImVec2(0, 1), ImVec2(1, 0));

			if (ImGui::IsWindowFocused())
			{
				editorCameraController = m_RenderProperties->GetCameraController(i);
				viewName = m_RenderProperties->GetViewName(i);
				m_RenderProperties->SetViewActive(i, true);
			}
			else
			{
				m_RenderProperties->SetViewActive(i, false);
			}
			if (viewName == m_RenderProperties->GetViewName(i))
			{		
				//Gizmos
				auto& camera = m_RenderProperties->GetCamera(i);
				const glm::mat4& cameraProjection = camera->GetProjectionMatrix();
				glm::mat4 cameraView = camera->GetViewMatrix();

				glm::mat4 identityMatrix = glm::mat4(1.f);

				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::AllowAxisFlip(false);


				Entity selectedEntity = m_HierarchyPanel->GetSelectedEntity();
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
				//m_RenderProperties->SetViewActive(i, true);
			}
			//else m_RenderProperties->SetViewActive(i, false);
			
			ImGui::End();
		}

		ImGui::End();


		m_HierarchyPanel->OnImGuiRender();
		m_EnvironmentPanel->OnImGuiRender();
		m_ContentBrowserPanel->OnImGuiRender();

		if (WindowMetricsEnabled)ImGui::ShowMetricsWindow();

		//Debug widow
		{
			ImGui::Begin("Debug");
			if (ImGui::SliderInt("samples of blur", &m_RenderProperties->GetScene()->skyLight->BlurSamples, 0, 50))
			{
				m_RenderProperties->GetScene()->skyLight->UpdateCubemap();
			}

			if (ImGui::SliderInt("view", &view, 0, framebuffers.size() - 1))
			{
				//editorCameraController->SetCamera(m_RenderProperties->GetCamera(view));
				//editorCameraController = m_RenderProperties->GetCameraController(view);
			}

			//ImGuiSliderFlags flags = ImGuiSliderFlags_None;
			ImGui::SliderFloat("Camera Speed", &editorCameraController->m_CameraSpeed, 1.f, 200.f);

			ImGui::SliderInt("shadow", &deferred, 0, 2);
			ImGui::Image((void*)m_RenderProperties->GetScene()->directionalLight->depthMap[deferred]->GetRendererID(), ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
			//ImGui::Image((void*)m_RenderProperties->GetDefferedFramebuffer()->GetColorAttachmentRendererID(deferred), ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
			//ImGui::Image((void*)debugFramebuffer->GetColorAttachmentRendererID(), ImVec2(secondCamera->ScreenWidth/3, secondCamera->ScreenHeight/3), ImVec2(0, 1), ImVec2(1, 0));

			ImGui::SliderFloat("far_plane", &m_RenderProperties->GetScene()->directionalLight->far_plane, -10, 200);
			ImGui::SliderFloat("near_plane", &m_RenderProperties->GetScene()->directionalLight->near_plane, -200, -10);
			ImGui::SliderFloat("size", &m_RenderProperties->GetScene()->directionalLight->size, 0, 200);

			ImGui::Checkbox("Use colors CSM", &csm);

			m_RenderProperties->GetDefferedShader()->Bind();
			m_RenderProperties->GetDefferedShader()->SetUniformInt("csmColor", csm);

			ImGui::End();
		}
	}

	void Editor::OnEvent(CrashEngine::Event& event)
	{
		if (!viewName.empty())editorCameraController->OnEvent(event);

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

		if (event.GetEventType() == EventType::WindowFocus)
		{
			CrashEngine::WindowFocusEvent& e = (CrashEngine::WindowFocusEvent&)event;

			for (int i = 0; m_RenderProperties->GetViewsNumber(); i++)
			{
				if (e.GetName() == m_RenderProperties->GetViewName(i))
				{
					
				}
			}
		
			
		}

		for (int i = 0; i < m_RenderProperties->GetViewsNumber(); i++)
		{
			if (m_RenderProperties->IsViewActive(i))
			{
				if (event.GetEventType() == EventType::MouseButtonPressed)
				{
					CrashEngine::MouseButtonPressedEvent& e = (CrashEngine::MouseButtonPressedEvent&)event;

					if (e.GetMouseButton() == 1)
					{
						Input::DisableCursor();
						isCursorVisible = false;
					}
				}

				if (event.GetEventType() == EventType::MouseButtonReleased)
				{
					CrashEngine::MouseButtonReleasedEvent& e = (CrashEngine::MouseButtonReleasedEvent&)event;
					if (e.GetMouseButton() == 1)
					{
						Input::EnableCursor();
						isCursorVisible = true;
					}
				}
			}		
		}		
	}

}