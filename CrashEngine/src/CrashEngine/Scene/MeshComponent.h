#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "CrashEngine/Renderer/Model.h"
#include "CrashEngine/Renderer/Texture.h"

namespace CrashEngine {

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
			UpdateModel();
		}

		void Draw(Shader* shader) { model->Draw(shader); }
		void UpdateModel()
		{
			for (int i = 0; i < model->meshes.size(); i++)
			{
				model->meshes[i].albedo = albedo;
				model->meshes[i].normal = normal;
				model->meshes[i].metallic = metallic;
				model->meshes[i].roughness = roughness;
				model->meshes[i].ao = ao;

				model->meshes[i].albedoTextureUse = albedoTextureUse;
				model->meshes[i].metallicTextureUse = metallicTextureUse;
				model->meshes[i].roughnessTextureUse = roughnessTextureUse;

				model->meshes[i].albedoColor = albedoColor;
				model->meshes[i].metallicValue = metallicValue;
				model->meshes[i].roughnessValue = roughnessValue;


			};
		}

		std::shared_ptr<Texture2D> albedo;
		std::shared_ptr<Texture2D> normal;
		std::shared_ptr<Texture2D> metallic;
		std::shared_ptr<Texture2D> roughness;
		std::shared_ptr<Texture2D> ao;

		bool albedoTextureUse;
		bool metallicTextureUse;
		bool roughnessTextureUse;


		glm::vec4 albedoColor = glm::vec4(0.2f, 0.2f, 0.2f, 0.0f);
		float metallicValue = 0.f;
		float roughnessValue = 0.5f;

	};

}