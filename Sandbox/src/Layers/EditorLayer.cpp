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

		editorCameraController.reset(new CameraController(glm::vec3(0.0f, 0.0f, 3.0f), Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()));

		//------------framebuffers--------------------
		FramebufferSpecification spec;
		spec.Height = Application::Get().GetWindow().GetHeight();
		spec.Width = Application::Get().GetWindow().GetWidth();

		framebuffer = Framebuffer::Create(spec);

		previewFramebuffer = Framebuffer::Create(spec);
		
		renderProperties.reset(new RenderProperties());

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


		editorCameraController->OnUpdate(ts);

		editorCameraController->GetCamera().SetSize(imguilayer->CurrentWindowView.y, imguilayer->CurrentWindowView.x);

		Renderer::RenderScene(renderProperties,framebuffer, &editorCameraController->GetCamera(),ts);


		Renderer::EndScene();
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

					HierarchyPanel.reset(new SceneHierarchyPanel(renderProperties->GetScene()));
					EnvironmentPanel.reset(new SceneEnvironmentPanel(renderProperties->GetScene()));
				}
				if (ImGui::MenuItem("Open"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("(*.crash)\0*.crash\0");
					if (filepath)
					{
						renderProperties.reset(new RenderProperties());

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
			ImGui::EndMainMenuBar();
		}


		imguilayer->Dockspace(framebuffer);

		//Gizmos
		auto& camera = editorCameraController->GetCamera();
		const glm::mat4& cameraProjection = camera.GetProjectionMatrix();
		glm::mat4 cameraView = camera.GetViewMatrix();

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
		
		ImGui::End();
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
		
		if (ImGui::Button("Show Info"))
		{

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
		//ImGui::Image((void*)debugFramebuffer->GetColorAttachmentRendererID(), ImVec2(400, 400), ImVec2(0, 1), ImVec2(1, 0));

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



	}

}