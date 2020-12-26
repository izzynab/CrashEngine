#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "CrashEngine/Renderer/MeshLoader.h"
#include "CrashEngine/Renderer/Texture.h"
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/TextureCreator.h"


namespace CrashEngine {

	struct MeshComponent
	{
		std::string path;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(std::string const& path);

		void Draw(Shader* shader);

		void SetupMesh();

		void LoadMesh(std::string const& path);

	public:
		std::vector<float>   vertices;
		std::vector<unsigned int> indices;

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

		std::shared_ptr<TextureCreator> texCreator;
	private:
		std::shared_ptr <VertexArray> VA;
		std::shared_ptr <VertexBuffer> VB;
		std::shared_ptr <IndexBuffer> IB;

	};

}