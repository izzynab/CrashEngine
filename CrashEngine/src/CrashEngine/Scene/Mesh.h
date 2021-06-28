#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "Material.h"
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
		ChildMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

		void Draw(Shader* shader);

		std::vector<float>       vertices;
		std::vector<unsigned int> indices;

	private:
		std::shared_ptr <VertexArray> VA;
		std::shared_ptr <VertexBuffer> VB;
		std::shared_ptr <IndexBuffer> IB;
	};


	class Mesh
	{
	public:
		std::vector<ChildMesh> meshes;
		std::string directory;
		std::string path;
		std::shared_ptr<Material> material;

	public:
		Mesh() = default;
		Mesh(const Mesh&) = default;
		Mesh(std::string const& path);
		Mesh(std::string const& path, std::shared_ptr<Material>& mat);

		void Draw(Shader* shader,bool bindTextures = true);

		void LoadMesh(std::string const& path);

		void processNode(aiNode* node, const aiScene* scene);

		ChildMesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::shared_ptr<Texture2D> LoadTexture(aiMaterial* mat,aiTextureType texturetype);
	};

}