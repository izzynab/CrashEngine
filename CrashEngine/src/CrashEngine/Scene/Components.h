#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "CrashEngine/Renderer/Model.h"
#include "CrashEngine/Renderer/Texture.h"

namespace CrashEngine {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct MeshComponent
	{
		Model* model;
		std::string path;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(Model* mod)
			:model(mod)
		{
			albedo = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_A.tga");
			normal = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_N.tga");
			metallic = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_M.tga");
			roughness = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_R.tga");
			ao = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_AO.tga");
		}

		void Draw(Shader* shader) { model->Draw(shader); }

		std::shared_ptr<Texture2D> albedo;
		std::shared_ptr<Texture2D> normal;
		std::shared_ptr<Texture2D> metallic;
		std::shared_ptr<Texture2D> roughness;
		std::shared_ptr<Texture2D> ao;

		bool albedoTextureUse;
		bool metallicTextureUse;
		bool roughnessTextureUse;


		glm::vec4 albedoColor = glm::vec4(0.2f, 0.2f, 0.2f,0.0f);
		float metallicValue = 0.f;
		float roughnessValue = 0.5f;

	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
}
