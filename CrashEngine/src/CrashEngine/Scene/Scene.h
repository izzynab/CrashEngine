#pragma once

#include <glm/glm.hpp>

#include "CrashEngine/Core/Timestep.h"

#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Framebuffer.h"
#include "CrashEngine/Renderer/BasicShapes.h"

#include "CrashEngine/PostProcess/PostProcess.h"

#include "CrashEngine/Scene/Material.h"
#include "CrashEngine/Scene/SSAO.h"
#include "CrashEngine/Scene/DirectionalLight.h"
#include "CrashEngine/Scene/SkyLight.h"

#include "entt.hpp"


namespace CrashEngine {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateMesh(const std::string& name = std::string());
		Entity CreateMesh(const std::string path, std::shared_ptr<Material> material, const std::string& name);
		Entity CreateLight(const std::string& name = std::string());

		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts, Shader* shader);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DepthRender(Shader* shader);
		
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	public:
		std::string filepath;

		std::shared_ptr<PostProcess> postProcess;
		std::shared_ptr<SSAO> ssao;

		std::shared_ptr<DirectionalLight> directionalLight;
		std::shared_ptr<SkyLight> skyLight;

	public://public for test
		entt::registry m_Registry;

	private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

		


	};

}
