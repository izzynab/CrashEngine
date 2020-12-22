#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.h"

namespace CrashEngine {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	class MeshLoader
	{
	public:
		MeshLoader() = default;

		MeshLoader(std::string const& path);

	private:
		void loadMeshData (aiMesh* mesh, const aiScene* scene);
		std::shared_ptr<Texture2D> LoadTexture(aiMaterial* mat, aiTextureType type);

	public:
		std::vector<Vertex>   vertices;
		std::vector<unsigned int> indices;

		std::shared_ptr<Texture2D> albedo;
		std::shared_ptr<Texture2D> normal;
		std::shared_ptr<Texture2D> metallic;
		std::shared_ptr<Texture2D> roughness;
		std::shared_ptr<Texture2D> ao;

	};
}