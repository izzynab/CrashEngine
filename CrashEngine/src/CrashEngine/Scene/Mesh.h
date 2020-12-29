#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "CrashEngine/Renderer/Texture.h"
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/TextureCreator.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace CrashEngine {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	class ChildMesh
	{
	public:
		ChildMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures);

		void Draw(Shader* shader);

		std::vector<float>       vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<Texture2D>>   textures;

	private:
		std::shared_ptr <VertexArray> VA;
		std::shared_ptr <VertexBuffer> VB;
		std::shared_ptr <IndexBuffer> IB;
	};


	class Mesh
	{
	public:
		std::vector<std::shared_ptr<Texture2D>> textures_loaded;
		std::vector<ChildMesh> meshes;
		std::string directory;

	public:
		Mesh() = default;
		Mesh(const Mesh&) = default;
		Mesh(std::string const& path);

		void Draw(Shader* shader);

		void LoadMesh(std::string const& path);

		void processNode(aiNode* node, const aiScene* scene);

		ChildMesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);//TODO: make material system

	public:
		/*std::shared_ptr<Texture2D> albedo;
		std::shared_ptr<Texture2D> normal;
		std::shared_ptr<Texture2D> metallic;
		std::shared_ptr<Texture2D> roughness;
		std::shared_ptr<Texture2D> ao;*/

	};

}