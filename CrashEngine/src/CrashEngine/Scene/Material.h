#pragma once

#include "CrashEngine/Renderer/Texture.h"
#include "CrashEngine/Renderer/TextureCreator.h"

namespace CrashEngine {

	class Material
	{
	public:
		Material(std::string name);

		void UpdateTexturesFromValues();

		std::string name;

		std::shared_ptr<Texture2D> albedo;
		std::shared_ptr<Texture2D> normal;
		std::shared_ptr<Texture2D> metallic;
		std::shared_ptr<Texture2D> roughness;
		std::shared_ptr<Texture2D> ao;

		std::shared_ptr<TextureCreator> texcreator;

		bool albedoTextureUse = false;
		bool metallicTextureUse = false;
		bool roughnessTextureUse = false;

		glm::vec4 albedoColor = glm::vec4(0.2f, 0.2f, 0.2f, 0.0f);
		glm::vec4 lastalbedoColor = glm::vec4(0.2f, 0.2f, 0.2f, 0.0f);
		float metallicValue = 0.5f;
		float roughnessValue = 0.5f;

		bool albedaLoad = false;
		bool metallicLoad = false;
		bool roughnessLoad = false;

	};
}