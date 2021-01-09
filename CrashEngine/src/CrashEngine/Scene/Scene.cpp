#include "cepch.h"

#include "Scene.h"
#include "Entity.h"

#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Utils/PlatformUtils.h"

#include "Components.h"
#include "Mesh.h"


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
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Mesh" : name;

		std::optional<std::string> filepath = FileDialogs::OpenFile("");
		if (filepath)
		{
			Mesh& mesh = Mesh(filepath.value());
			mesh.directory = filepath.value();
			entity.AddComponent<Mesh>(mesh);
		}
		return entity;
	}

	Entity Scene::CreateLight(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Light" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto view = m_Registry.view<TransformComponent, Mesh>();

		for (auto entity : view) 
		{
			auto& mod = view.get<Mesh>(entity);
			auto &transform = view.get<TransformComponent>(entity);

			glm::mat4 model = transform.GetTransform();
			defaultShader->SetUniformMat4("model", model);

			mod.Draw(defaultShader);	
		}

		
	}

	void Scene::DepthRender()
	{
		auto view = m_Registry.view<TransformComponent, Mesh>();

		for (auto entity : view)
		{
			auto& mod = view.get<Mesh>(entity);
			auto& transform = view.get<TransformComponent>(entity);

			glm::mat4 model = transform.GetTransform();
			depthShader->SetUniformMat4("model", model);

			mod.Draw(depthShader,false);//todo: depth render couses errors 
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

	template<>
	void Scene::OnComponentAdded<Mesh>(Entity entity, Mesh& component)
	{
		
	}


}
