#include "cepch.h"
#include "Material.h"


namespace CrashEngine {

	Material::Material(std::string name)
		:name(name)
	{
		texcreator.reset(new TextureCreator());

		albedo = texcreator->CreateTexture(200, 200, albedoColor);
		normal = texcreator->CreateTexture(200,200, glm::vec3(0.5, 0.5, 1));
		metallic = texcreator->CreateTexture(200, 200, glm::vec3(metallicValue));
		roughness = texcreator->CreateTexture(200, 200, glm::vec3(roughnessValue));
		ao = texcreator->CreateTexture(200, 200, glm::vec3(1, 1, 1));
	}

	void Material::UpdateTexturesFromValues()
	{
		if (albedoTextureUse)
		{
			albedo = albedo->Create(albedo->m_Path);
		}
		else
		{
			albedo = texcreator->CreateTexture(200, 200, albedoColor);
		}
		if (metallicTextureUse)
		{
			metallic = metallic->Create(metallic->m_Path);
		}
		else
		{
			metallic = texcreator->CreateTexture(200, 200, glm::vec3(metallicValue));
		}
		if (roughnessTextureUse)
		{
			roughness = roughness->Create(roughness->m_Path);
		}
		else
		{
			roughness = texcreator->CreateTexture(200, 200, glm::vec3(roughnessValue));
		}
		if (!normal->m_Path.empty())
		{
			normal = normal->Create(normal->m_Path);
		}
		if (!ao->m_Path.empty())
		{
			ao = ao->Create(ao->m_Path);
		}
	}
}