#pragma once

#include "CrashEngine/Core/Timestep.h"
#include "CrashEngine/Renderer/Shader.h"

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
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		inline void SetDefaultShader(Shader* shader) { defaultShader = shader; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	public://public for test
		entt::registry m_Registry;

	private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

		Shader *defaultShader;


	};

}
