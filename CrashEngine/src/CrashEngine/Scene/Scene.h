#pragma once

#include "CrashEngine/Core/Timestep.h"
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Framebuffer.h"
#include "CrashEngine/Renderer/BasicShapes.h"
#include "CrashEngine/Scene/Material.h"

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

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DepthRender();
		void BlurRender();

		inline void SetDefaultShader(Shader* shader) { defaultShader = shader; }
		inline void SetDepthShader(Shader* shader) { depthShader = shader; }
		inline void SetFramebuffers(std::shared_ptr<Framebuffer>& MSAAFramebuffer, std::shared_ptr<Framebuffer> &FinalFramebuffer) { framebuffer = MSAAFramebuffer, finalFramebuffer = FinalFramebuffer; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	public://public for test
		entt::registry m_Registry;
		std::shared_ptr<Framebuffer> framebuffer;
		std::shared_ptr<Framebuffer> finalFramebuffer;

		bool blur = true;
		float exposure = 1.f;
	private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

		Shader *defaultShader;
		Shader* depthShader;

		Shader* blurShader;
		Shader* BloomMixShader;
		Shader* brightShader;
		std::shared_ptr<Framebuffer> blurFramebuffer[2];
		std::shared_ptr<Framebuffer> bloom;
		std::shared_ptr<Framebuffer> draw_framebuffer;
		std::shared_ptr<Quad> quad;


	};

}
