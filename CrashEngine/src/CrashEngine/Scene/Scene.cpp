#include "cepch.h"

#include "Scene.h"
#include "Entity.h"

#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Utils/PlatformUtils.h"
#include "CrashEngine/Core/Defines.h"

#include "Components.h"
#include "Mesh.h"
#include "PointLight.h"


namespace CrashEngine {

	Scene::Scene()
	{
		postProcess.reset(new PostProcess());
		ssao.reset(new SSAO());

		skyLight.reset(new SkyLight);

		directionalLight.reset(new DirectionalLight);
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

	Entity Scene::CreateMesh(const std::string path, std::shared_ptr<Material> material, const std::string& name = std::string())
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Mesh" : name;


		Mesh& mesh = Mesh(path);
		mesh.directory = path;
		mesh.material = material;
		entity.AddComponent<Mesh>(mesh);
		
		return entity;
	}

	Entity Scene::CreateLight(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Light" : name;

		PointLight& pointLight = PointLight();
		entity.AddComponent<PointLight>();

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts, Shader* shader)
	{
		auto view = m_Registry.view<TransformComponent, Mesh>();

		for (auto entity : view) 
		{
			auto& mod = view.get<Mesh>(entity);
			auto &transform = view.get<TransformComponent>(entity);

			glm::mat4 model = transform.GetTransform();

			shader->Bind();
			shader->SetUniformMat4("model", model);

			mod.Draw(shader);		

		}

		auto instancedview = m_Registry.view<TransformComponent, InstanedMeshes>();

		for (auto entity : instancedview)
		{
			//todo: implement instanced mesh render

		}
	}

	void Scene::DepthRender(Shader* shader)
	{
		auto view = m_Registry.view<TransformComponent, Mesh>();

		for (auto entity : view)
		{
			auto& mod = view.get<Mesh>(entity);
			auto& transform = view.get<TransformComponent>(entity);

			glm::mat4 model = transform.GetTransform();
			shader->Bind();
			shader->SetUniformMat4("model", model);

			mod.Draw(shader,false);
		}
		//todo: add isntanced meshes render pass

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

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		//component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Mesh>(Entity entity, Mesh& component)
	{
		
	}

	template<>
	void Scene::OnComponentAdded<PointLight>(Entity entity, PointLight& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<InstanedMeshes>(Entity entity, InstanedMeshes& component)
	{

	}


}
