#include "cepch.h"

#include "Scene.h"
#include "Entity.h"

#include "CrashEngine/Renderer/Renderer.h"

#include "Components.h"
#include "MeshComponent.h"


namespace CrashEngine {

	Scene::Scene()
	{	
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	Entity Scene::CreateMesh(const std::string& name)
	{
		
	}

	Entity Scene::CreateLight(const std::string& name)
	{

	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto view = m_Registry.view<TransformComponent, MeshComponent>();

		for (auto entity : view) 
		{
			auto& mod = view.get<MeshComponent>(entity);
			auto &transform = view.get<TransformComponent>(entity);

			glm::mat4 model = transform.GetTransform();
			defaultShader->SetUniformMat4("model", model);

			mod.Draw(defaultShader);	
		}

		
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	/*template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}*/

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}


}
