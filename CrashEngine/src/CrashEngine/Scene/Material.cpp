#include "cepch.h"
#include "Material.h"


namespace CrashEngine {

	Material::Material(std::string name)
	{
		this->name.reset(new std::string(name));

		texcreator.reset(new TextureCreator());

		albedo = texcreator->CreateTexture(200, 200, glm::vec3(0.4, 0.4, 0.4));
		normal = texcreator->CreateTexture(200,200, glm::vec3(0.5, 0.5, 1));
		metallic = texcreator->CreateTexture(200, 200, glm::vec3(0.5, 0.5, 0.5));
		roughness = texcreator->CreateTexture(200, 200, glm::vec3(0.5, 0.5, 0.5));
		ao = texcreator->CreateTexture(200, 200, glm::vec3(1, 1, 1));
	}
}