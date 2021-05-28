#include "cepch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"
#include "Mesh.h"
#include "DirectionalLight.h"
#include "SkyLight.h"

#include <fstream>
#include "yaml-cpp/yaml.h"


namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}
namespace CrashEngine {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene)
		: m_Scene(scene)
	{
		m_skyLight = scene->skyLight;
		m_directionalLight = scene->directionalLight;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<Mesh>())
		{
			out << YAML::Key << "Mesh";
			out << YAML::BeginMap; // Mesh

			auto& tc = entity.GetComponent<Mesh>();
			out << YAML::Key << "Directory" << YAML::Value << tc.path;


			out << YAML::Key << "Material";
			out << YAML::BeginMap; // Material

			//auto& tc = entity.GetComponent<Mesh>();
			out << YAML::Key << "Name" << YAML::Value << tc.material->name;

			out << YAML::Key << "Albedo";
			out << YAML::BeginMap;
			out << YAML::Key << "path" << YAML::Value << tc.material->albedo->m_Path;
			out << YAML::Key << "albedo use" << YAML::Value << tc.material->albedoTextureUse;
			out << YAML::Key << "albedo color" << YAML::Value << tc.material->albedoColor;
			out << YAML::Key << "albedo load" << YAML::Value << tc.material->albedaLoad;
			out << YAML::EndMap;

			out << YAML::Key << "Metallic";
			out << YAML::BeginMap;
			out << YAML::Key << "path" << YAML::Value << tc.material->metallic->m_Path;
			out << YAML::Key << "metallic use" << YAML::Value << tc.material->metallicTextureUse;
			out << YAML::Key << "metallic color" << YAML::Value << tc.material->metallicValue;
			out << YAML::Key << "metallic load" << YAML::Value << tc.material->metallicLoad;
			out << YAML::EndMap;

			out << YAML::Key << "Roughness";
			out << YAML::BeginMap;
			out << YAML::Key << "path" << YAML::Value << tc.material->roughness->m_Path;
			out << YAML::Key << "roughness use" << YAML::Value << tc.material->roughnessTextureUse;
			out << YAML::Key << "roughness color" << YAML::Value << tc.material->roughnessValue;
			out << YAML::Key << "roughness load" << YAML::Value << tc.material->roughnessLoad;
			out << YAML::EndMap;

			out << YAML::Key << "Normal";
			out << YAML::BeginMap;
			out << YAML::Key << "path" << YAML::Value << tc.material->normal->m_Path;
			out << YAML::EndMap;

			out << YAML::Key << "Ao";
			out << YAML::BeginMap;
			out << YAML::Key << "path" << YAML::Value << tc.material->ao->m_Path;
			out << YAML::EndMap;

			out << YAML::EndMap; // Material



			out << YAML::EndMap; // Mesh
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& camera = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "Primary" << YAML::Value << camera.Primary;
			out << YAML::Key << "Draw Frustum" << YAML::Value << camera.DrawFrustum;

			out << YAML::Key << "Rotation" << YAML::Value << camera.Camera->Rotation;
			out << YAML::Key << "Position" << YAML::Value << camera.Camera->Position;

			out << YAML::EndMap; // TransformComponent
		}

		out << YAML::EndMap; // Entity
	}

	static void SeriazlizeEnvironment(YAML::Emitter& out, std::shared_ptr<DirectionalLight> &m_directionalLight, std::shared_ptr<SkyLight> &m_skyLight, std::shared_ptr<Scene>& m_Scene)
	{
		out << YAML::Key << "Directional light" << YAML::Value;
		out << YAML::BeginMap; // Directional light

		out << YAML::Key << "rotation" << YAML::Value << m_directionalLight->rotation;
		out << YAML::Key << "color" << YAML::Value << m_directionalLight->color;
		out << YAML::Key << "intensity" << YAML::Value << m_directionalLight->intensity;

		out << YAML::EndMap; // Directional light


		out << YAML::Key << "Sky light" << YAML::Value;
		out << YAML::BeginMap; // Sky light

		out << YAML::Key << "directory" << YAML::Value << m_skyLight->directory;

		out << YAML::EndMap; // Sky light

		out << YAML::Key << "Bloom" << YAML::Value;
		out << YAML::BeginMap; // blur

		out << YAML::Key << "blur" << YAML::Value << m_Scene->postProcess->blur;
		out << YAML::Key << "threshold" << YAML::Value << m_Scene->postProcess->blurThreshold;
		out << YAML::Key << "softthreshold" << YAML::Value << m_Scene->postProcess->blurSoftThreshold;

		out << YAML::EndMap; // blur

		out << YAML::Key << "SSAO" << YAML::Value;
		out << YAML::BeginMap; // SSAO

		out << YAML::Key << "radius" << YAML::Value << m_Scene->ssao->radius;
		out << YAML::Key << "bias" << YAML::Value << m_Scene->ssao->bias;
		out << YAML::Key << "power" << YAML::Value << m_Scene->ssao->power;

		out << YAML::EndMap; // blur


	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Filepath" << YAML::Value << m_Scene->filepath;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;

		SeriazlizeEnvironment(out, m_directionalLight, m_skyLight,m_Scene);

		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
		CE_CORE_ASSERT(false);
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		CE_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		m_Scene->filepath = data["Filepath"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				Entity deserializedEntity;
				
				auto mesh = entity["Mesh"];
				if (mesh)
				{
					std::string path = mesh["Directory"].as<std::string>();

					auto material = mesh["Material"];

					std::string matname = material["Name"].as<std::string>();
					std::shared_ptr<Material> mat = std::make_shared<Material>(matname);

					auto albedo = material["Albedo"];
					mat->albedo->m_Path = albedo["path"].as<std::string>();
					mat->albedoTextureUse = albedo["albedo use"].as<bool>();
					mat->albedoColor = albedo["albedo color"].as<glm::vec4>();
					mat->albedaLoad = albedo["albedo load"].as<bool>();

					auto metallic = material["Metallic"];
					mat->metallic->m_Path = metallic["path"].as<std::string>();
					mat->metallicTextureUse = metallic["metallic use"].as<bool>();
					mat->metallicValue = metallic["metallic color"].as<float>();
					mat->metallicLoad = metallic["metallic load"].as<bool>();

					auto roughness = material["Roughness"];
					mat->roughness->m_Path = roughness["path"].as<std::string>();
					mat->roughnessTextureUse = roughness["roughness use"].as<bool>();
					mat->roughnessValue = roughness["roughness color"].as<float>();
					mat->roughnessLoad = roughness["roughness load"].as<bool>();

					auto normal = material["Normal"];
					mat->normal->m_Path = normal["path"].as<std::string>();

					auto ao = material["Ao"];
					mat->ao->m_Path = ao["path"].as<std::string>();

					mat->UpdateTexturesFromValues();

					deserializedEntity = m_Scene->CreateMesh(path, mat, name);

				}
				else
				{
					//deserializedEntity = m_Scene->CreateMesh(name);
				}
			
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();

					auto cameraComponent = entity["CameraComponent"];
					if (cameraComponent)
					{
						// Entities always have transforms
						CameraComponent camera;

						camera.Primary = cameraComponent["Primary"].as<bool>();
						camera.DrawFrustum = cameraComponent["Draw Frustum"].as<bool>();

						//camera.Camera->Rotation = cameraComponent["Rotation"].as<glm::vec3>();
						//camera.Camera->Position = cameraComponent["Position"].as<glm::vec3>();

						camera.Camera->UpdatePosition(cameraComponent["Position"].as<glm::vec3>(), tc.Translation);
						camera.Camera->UpdateRotation(cameraComponent["Rotation"].as<glm::vec3>(), tc.Translation);

						deserializedEntity.AddComponent<CameraComponent>(camera);
					}
				}
			}
		}

		auto directlight = data["Directional light"];

		m_directionalLight->rotation = directlight["rotation"].as<glm::vec3>();
		m_directionalLight->color = directlight["color"].as<glm::vec3>();
		m_directionalLight->intensity = directlight["intensity"].as<float>();

		auto skyLight = data["Sky light"];

		std::string dir = skyLight["directory"].as<std::string>();
		m_skyLight->LoadHDR(dir);

		auto bloom = data["Bloom"];
		m_Scene->postProcess->blur = bloom["blur"].as<bool>();
		m_Scene->postProcess->blurThreshold = bloom["threshold"].as<float>();
		m_Scene->postProcess->blurSoftThreshold = bloom["softthreshold"].as<float>();

		auto ssao = data["SSAO"];
		m_Scene->ssao->radius = ssao["radius"].as<float>();
		m_Scene->ssao->bias = ssao["bias"].as<float>();
		m_Scene->ssao->power = ssao["power"].as<float>();

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented
		CE_CORE_ASSERT(false);
		return false;
	}
}