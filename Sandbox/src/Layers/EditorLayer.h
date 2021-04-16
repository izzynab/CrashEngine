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

#include "CrashEngine/PostProcess/PostProcess.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/SceneEnvironmentPanel.h"
#include "Panels/MaterialPanel.h"

#include "CrashEngine/Debug/LinesDrawer.h"


namespace CrashEngine {

	class Editor : public CrashEngine::Layer
	{
	public:
		Editor();

		void OnUpdate(Timestep ts) override;

		virtual void OnImGuiRender() override;

		void OnEvent(CrashEngine::Event& event) override;

	private:
		std::shared_ptr<Scene> m_ActiveScene;

		std::shared_ptr<ImGuiLayer> imguilayer;
		std::shared_ptr <SceneHierarchyPanel> HierarchyPanel;
		std::shared_ptr <SceneEnvironmentPanel> EnvironmentPanel;

		std::shared_ptr<UniformBuffer> m_MatrixUB;
		std::shared_ptr<CameraController> cameraController;

		glm::mat4 model;

		std::shared_ptr<Sphere> sphere;
		std::shared_ptr<Cube> cube;
		std::shared_ptr<Quad> quad;

		std::shared_ptr<DebugLine> debugLine;

		std::shared_ptr<SkyLight> skyLight;
		std::shared_ptr<DirectionalLight> directionalLight;

		//std::shared_ptr<PostProcess> postProcess;

		/*GBuffer shader gives buffers to deffered render*/
		Shader* GBufferShader;

		/*Shader draws pbr scene from gbuffer images*/
		Shader* deferredShader;

		/*Shader draws pbr scene from textures of models*/
		Shader* forwardShader;

		/*Last framebuffer which is shown in ImGui Dockspace Image*/
		std::shared_ptr<Framebuffer> framebuffer;

		/*Framebuffer which stores GBuffer images*/
		std::shared_ptr<Framebuffer> deferredframebuffer;

		float Height;
		float Width;

		int gizmoType = 7;

		int deferred = 0;
		bool metrics = false;
		bool forward = false;
		bool csm = false;
	};

}