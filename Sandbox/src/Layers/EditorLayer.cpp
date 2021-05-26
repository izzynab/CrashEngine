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

		imguilayer.reset(new ImGuiLayer);

		//------------framebuffers--------------------

		FramebufferSpecification spec;
		spec.Height = Application::Get().GetWindow().GetHeight();
		spec.Width = Application::Get().GetWindow().GetWidth();
 
		renderProperties.reset(new RenderProperties());

		Renderer::AddView(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), "Main", renderProperties, framebuffers);

		editorCameraController = renderProperties->GetCameraController(0);


		HierarchyPanel.reset(new SceneHierarchyPanel(renderProperties->GetScene()));
		EnvironmentPanel.reset(new SceneEnvironmentPanel(renderProperties->GetScene()));

		SceneSerializer serializer(renderProperties->GetScene());
		serializer.Deserialize("C:/EngineDev/CrashEngine/Models/Scenes/jd.crash");


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
		Renderer::BeginScene();

		auto camera = renderProperties->GetScene()->m_Registry.view<TagComponent,CameraComponent>();

		for (auto entity : camera)
		{
			if (camera.get<CameraComponent>(entity).DrawFrustum == true)
			{
				Application::Get().GetDebugger().DrawFrustum(camera.get<CameraComponent>(entity).Camera.get());
				//CE_TRACE("Camera frustum draw call: {0}", camera.get<TagComponent>(entity).Tag);
			}
		}

		Application::Get().GetDebugger().DrawFrustum(renderProperties->GetCamera(0).get());

		if(!viewName.empty()) editorCameraController->OnUpdate(ts);

		Renderer::RenderScene(renderProperties, framebuffers,ts);

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
		//imguilayer->MainMenu();
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Hello, it's my menu", NULL, false, false);
				if (ImGui::MenuItem("New"))
				{
					renderProperties.reset(new RenderProperties());

					Renderer::AddView(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), "Main", renderProperties, framebuffers);

					editorCameraController = renderProperties->GetCameraController(0);

					HierarchyPanel.reset(new SceneHierarchyPanel(renderProperties->GetScene()));
					EnvironmentPanel.reset(new SceneEnvironmentPanel(renderProperties->GetScene()));
				}
				if (ImGui::MenuItem("Open"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("(*.crash)\0*.crash\0");
					if (filepath)
					{
						renderProperties.reset(new RenderProperties());

						Renderer::AddView(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), "Main", renderProperties, framebuffers);

						editorCameraController = renderProperties->GetCameraController(0);

						HierarchyPanel.reset(new SceneHierarchyPanel(renderProperties->GetScene()));
						EnvironmentPanel.reset(new SceneEnvironmentPanel(renderProperties->GetScene()));

						SceneSerializer serializer(renderProperties->GetScene());
						serializer.Deserialize(filepath.value());
					}
				}

				if (ImGui::MenuItem("Save")) 
				{
					std::optional<std::string> filepath = renderProperties->GetScene()->filepath;
					if (filepath)
					{
						SceneSerializer serializer(renderProperties->GetScene());
						serializer.Serialize(filepath.value());
					}
					else
					{
						std::optional<std::string> filepathAs = FileDialogs::SaveFile("(*.crash)\0*.crash\0");
						if (filepathAs)
						{
							renderProperties->GetScene()->filepath = filepathAs.value();
							SceneSerializer serializer(renderProperties->GetScene());
							serializer.Serialize(filepathAs.value());
						}
					}
				}
				if (ImGui::MenuItem("Save As.."))
				{
					std::optional<std::string> filepath = FileDialogs::SaveFile("(*.crash)\0*.crash\0");
					if (filepath)
					{
						renderProperties->GetScene()->filepath = filepath.value();
						SceneSerializer serializer(renderProperties->GetScene());
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
					if(!renderProperties->views.empty())Renderer::AddView(800, 800, "View " + std::to_string(renderProperties->views.back().id), renderProperties, framebuffers);
					else Renderer::AddView(800, 800, "Main", renderProperties, framebuffers);
				}

				ImGui::EndMenu();		
			}
			ImGui::EndMainMenuBar();
		}

		imguilayer->Dockspace(renderProperties, framebuffers);

		if (!isCursorVisible)
		{
			ImGui::SetWindowFocus(viewName.c_str());
		}

		//viewName = std::string();

		for (int i = 0; i < renderProperties->GetViewsNumber(); i++)
		{
			if (!renderProperties->views[i].isOpen)
			{
				renderProperties->views.erase(renderProperties->views.begin() + i);
				i--;
				continue;
			}

			ImGui::Begin(renderProperties->GetViewName(i).c_str(), &renderProperties->views[i].isOpen);

			ImVec2 CurrentWindowView = ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 40);
			renderProperties->GetCamera(i)->SetSize(CurrentWindowView.y, CurrentWindowView.x);

			ImVec2 imageSize = ImVec2(framebuffers[i]->GetSpecification().Width, framebuffers[i]->GetSpecification().Height);

			ImGui::Image((void*)framebuffers[i]->GetColorAttachmentRendererID(0), imageSize, ImVec2(0, 1), ImVec2(1, 0));

			if (ImGui::IsWindowFocused())
			{
				editorCameraController = renderProperties->GetCameraController(i);
				viewName = renderProperties->GetViewName(i);
			}

			if (viewName == renderProperties->GetViewName(i))
			{		
				//Gizmos
				auto& camera = renderProperties->GetCamera(i);
				const glm::mat4& cameraProjection = camera->GetProjectionMatrix();
				glm::mat4 cameraView = camera->GetViewMatrix();

				glm::mat4 identityMatrix = glm::mat4(1.f);

				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::AllowAxisFlip(false);


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
				renderProperties->SetViewActive(i, true);
			}
			else renderProperties->SetViewActive(i, false);
			
			ImGui::End();
		}

		ImGui::End();

		if (imguilayer->MenuEnabled) { imguilayer->Menu(); }
		if (imguilayer->WindowMetricsEnabled) { imguilayer->WindowMetrics(); }
		if (imguilayer->EditorStyleEnabled) { imguilayer->StyleEditor(); }


		HierarchyPanel->OnImGuiRender();
		EnvironmentPanel->OnImGuiRender();


		ImGui::Begin("Debug");
		if (ImGui::SliderInt("samples of blur", &renderProperties->GetScene()->skyLight->BlurSamples, 0, 50))
		{
			renderProperties->GetScene()->skyLight->UpdateCubemap();
		}
		
		if (ImGui::SliderInt("view", &view, 0, framebuffers.size() - 1))
		{
			//editorCameraController->SetCamera(renderProperties->GetCamera(view));
			//editorCameraController = renderProperties->GetCameraController(view);
		}

		ImGui::SliderFloat("Camera Speed", &editorCameraController->m_CameraSpeed, 1.f, 100.f);
		ImGui::Checkbox("ssao", &forward);

		
		ImGui::Checkbox("metrics", &metrics);
		if (metrics)
		{
			imguilayer->WindowMetrics();

	}
		

		ImGui::SliderInt("shadow", &deferred, 0, 2);
		ImGui::Image((void*)renderProperties->GetScene()->directionalLight->depthMap[deferred]->GetRendererID(), ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::Image((void*)renderProperties->GetDefferedFramebuffer()->GetColorAttachmentRendererID(deferred), ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::Image((void*)debugFramebuffer->GetColorAttachmentRendererID(), ImVec2(secondCamera->ScreenWidth/3, secondCamera->ScreenHeight/3), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::SliderFloat("far_plane", &renderProperties->GetScene()->directionalLight->far_plane, -10, 200);
		ImGui::SliderFloat("near_plane", &renderProperties->GetScene()->directionalLight->near_plane, -200, -10);
		ImGui::SliderFloat("size", &renderProperties->GetScene()->directionalLight->size, 0, 200);

		ImGui::Checkbox("Use colors CSM", &csm);

		renderProperties->GetDefferedShader()->Bind();
		renderProperties->GetDefferedShader()->SetUniformInt("csmColor", csm);

		ImGui::End();
	
	}

	void Editor::OnEvent(CrashEngine::Event& event)
	{
		editorCameraController->OnEvent(event);

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

			for (int i = 0; renderProperties->GetViewsNumber(); i++)
			{
				if (e.GetName() == renderProperties->GetViewName(i))
				{
					
				}
			}
		
			
		}

		for (int i = 0; i < renderProperties->GetViewsNumber(); i++)
		{
			if (renderProperties->IsViewActive(i))
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