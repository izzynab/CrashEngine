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
		std::shared_ptr<Scene> m_ActiveScene;
		Entity m_Entity;

		std::shared_ptr<ImGuiLayer> imguilayer;
		std::shared_ptr <SceneHierarchyPanel> HierarchyPanel;
		std::shared_ptr <SceneEnvironmentPanel> EnvironmentPanel;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<UniformBuffer> m_MatrixUB;
		std::shared_ptr<CameraController> cameraController;

		Shader* basicShader;
		Shader* pbrTextureShader;
		Shader* depthMapShader;

		glm::mat4 model;

		std::shared_ptr<Sphere> sphere;
		std::shared_ptr<Cube> cube;
		std::shared_ptr<Quad> quad;

		std::shared_ptr<Framebuffer> framebuffer;


		std::shared_ptr<SkyLight> skyLight;
		std::shared_ptr<DirectionalLight> directionalLight;


		std::shared_ptr<Framebuffer> depthFramebuffer;
		std::vector<std::shared_ptr<DepthTexture>> depthMap;
		int cascademapselected = 1;

		float Height;
		float Width;

		std::vector<float> m_cascadeEnd;
	};

}