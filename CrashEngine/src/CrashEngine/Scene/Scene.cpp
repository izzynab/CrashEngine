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
		quad.reset(new Quad());

		FramebufferSpecification spec;
		spec.Height = 1080;
		spec.Width = 2560;

		//----------------blur-------------------------
		blurFramebuffer[0] = Framebuffer::Create(spec);
		blurFramebuffer[1] = Framebuffer::Create(spec);

		blurShader = Shader::Create("Basic.vert", "blur.frag");
		BloomMixShader = Shader::Create("Basic.vert", "mixbloom.frag");
		BloomMixShader->Bind();
		BloomMixShader->SetUniformInt("scene", 0);
		BloomMixShader->SetUniformInt("bloomBlur", 1);
		//----------------blur-------------------------

		//RenderCommand::Enable(CE_MULTISAMPLE);
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

	void Scene::BlurRender()
	{
		// 2. blur bright fragments with two-pass Gaussian Blur 
		bool horizontal = true, first_iteration = true;
		unsigned int amount = 10;
		blurShader->Bind();
		for (unsigned int i = 0; i < amount; i++)
		{
			blurFramebuffer[horizontal]->Bind();
			blurFramebuffer[horizontal]->SetNewTexture(framebuffer->GetSpecification().Width, framebuffer->GetSpecification().Height);
	
			RenderCommand::Clear();
			blurShader->SetUniformInt("horizontal", horizontal);

			RenderCommand::BindTexture(first_iteration ? framebuffer->GetColorAttachmentRendererID(1) : blurFramebuffer[!horizontal]->GetColorAttachmentRendererID(), 0);
			quad->RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
			blurFramebuffer[horizontal]->Unbind();
		}


		// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		RenderCommand::Clear();
		BloomMixShader->Bind();
		RenderCommand::BindTexture(framebuffer->GetColorAttachmentRendererID(), 0);
		RenderCommand::BindTexture(blurFramebuffer[1]->GetColorAttachmentRendererID(), 1);
		BloomMixShader->SetUniformInt("exposure", exposure);
		BloomMixShader->SetUniformInt("blur", blur);
		framebuffer->Bind();
		quad->RenderQuad();
		framebuffer->Unbind();
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

	template<>
	void Scene::OnComponentAdded<PointLight>(Entity entity, PointLight& component)
	{

	}


}
