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
#include "Panels/ContentBrowserPanel.h"




namespace CrashEngine {

	class Editor : public CrashEngine::Layer
	{
	public:
		Editor();

		void OnUpdate(Timestep ts) override;

		virtual void OnImGuiRender() override;

		void OnEvent(CrashEngine::Event& event) override;

	private:
		//std::shared_ptr<ImGuiLayer> imguilayer;
		std::shared_ptr <SceneHierarchyPanel> m_HierarchyPanel;
		std::shared_ptr <SceneEnvironmentPanel> m_EnvironmentPanel;
		std::shared_ptr <ContentBrowserPanel> m_ContentBrowserPanel;
		
		std::shared_ptr<RenderProperties> m_RenderProperties;

		std::shared_ptr<Sphere> m_Sphere;
		std::shared_ptr<Cube> m_Cube;
		std::shared_ptr<Quad> m_Quad;

		Shader* QuadShader;

		/*Last framebuffer which is shown in ImGui Dockspace Image*/
		std::vector<std::shared_ptr<Framebuffer>> framebuffers;

		/*framebuffer used to debug purposes*/
		std::shared_ptr<Framebuffer> debugFramebuffer;

		std::shared_ptr<CameraController> editorCameraController;
		//std::shared_ptr<Camera> editorCamera;

		int gizmoType = 7;
		std::string viewName;
		bool isCursorVisible = true;
		bool WindowMetricsEnabled = false;

		//----------------test--------------------

		int view = 0;
		int deferred = 0;
		bool csm = false;

		std::shared_ptr<Texture2D> testTexture;
		ComputeShader* compShader;
	};

}