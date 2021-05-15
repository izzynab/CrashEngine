#pragma once

#include <Crash.h>

#include "imgui/imgui.h"
#include "CrashEngine/Core/Timestep.h"

#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"

#include "CrashEngine/Scene/Entity.h"
#include "CrashEngine/Scene/Scene.h"
#include "CrashEngine/Scene/Components.h"
#include "CrashEngine/Scene/Mesh.h"

#include "CrashEngine/Renderer/RenderProperties.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/SceneEnvironmentPanel.h"
#include "Panels/MaterialPanel.h"



namespace CrashEngine {

	class Editor : public CrashEngine::Layer
	{
	public:
		Editor();

		void OnUpdate(Timestep ts) override;

		virtual void OnImGuiRender() override;

		void OnEvent(CrashEngine::Event& event) override;

	private:
		std::shared_ptr<ImGuiLayer> imguilayer;
		std::shared_ptr <SceneHierarchyPanel> HierarchyPanel;
		std::shared_ptr <SceneEnvironmentPanel> EnvironmentPanel;

		std::shared_ptr<CameraController> editorCameraController;

		std::shared_ptr<RenderProperties> renderProperties;

		//std::shared_ptr<Camera> testCamera;

		std::shared_ptr<Sphere> sphere;
		std::shared_ptr<Cube> cube;
		std::shared_ptr<Quad> quad;


		Shader* QuadShader;

		/*Last framebuffer which is shown in ImGui Dockspace Image*/
		std::shared_ptr<Framebuffer> framebuffer;

		/*Framebuffer which is used to show second camera view to small ImGui window*/
		std::shared_ptr<Framebuffer> previewFramebuffer;

		/*framebuffer used to debug purposes*/
		std::shared_ptr<Framebuffer> debugFramebuffer;

		float previewHeight = 600.f;
		float previewWidth = 600.f;

		int gizmoType = 7;

		int deferred = 0;
		bool metrics = false;
		bool forward = false;
		bool csm = false;

		std::shared_ptr<Texture2D> testTexture;
		ComputeShader* compShader;
	};

}