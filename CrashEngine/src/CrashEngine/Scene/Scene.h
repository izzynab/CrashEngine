#pragma once

#include "CrashEngine/Core/Timestep.h"
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Framebuffer.h"
#include "CrashEngine/Renderer/BasicShapes.h"
#include "CrashEngine/Scene/Material.h"
#include "CrashEngine/PostProcess/PostProcess.h"

#include <glm/glm.hpp>

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

		void OnUpdate(Timestep ts, Shader* shader = nullptr);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DepthRender();

		inline void SetDefaultShader(Shader* shader) { defaultShader = shader; }
		inline void SetDepthShader(Shader* shader) { depthShader = shader; }
		
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	public:
		std::shared_ptr<PostProcess> postProcess;

	public://public for test
		entt::registry m_Registry;

	private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

		Shader* defaultShader;
		Shader* depthShader;

		


	};

}
