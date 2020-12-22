#include "cepch.h"
#include "MeshComponent.h"

#include "CrashEngine/Renderer/Renderer.h"

namespace CrashEngine {

	MeshComponent::MeshComponent(std::string const& path)
		: path(path)
	{
		LoadMesh(path);
	}

	void MeshComponent::LoadMesh(std::string const& path)
	{
		MeshLoader loader = MeshLoader(path);

		//vertices = loader.vertices;

		for (Vertex i : loader.vertices)
		{
			vertices.push_back(i.Position.x);
			vertices.push_back(i.Position.y);
			vertices.push_back(i.Position.z);

			vertices.push_back(i.TexCoords.x);
			vertices.push_back(i.TexCoords.y);

			vertices.push_back(i.Normal.x);
			vertices.push_back(i.Normal.y);
			vertices.push_back(i.Normal.z);

			vertices.push_back(i.Tangent.x);
			vertices.push_back(i.Tangent.y);
			vertices.push_back(i.Tangent.z);

			vertices.push_back(i.Bitangent.x);
			vertices.push_back(i.Bitangent.y);
			vertices.push_back(i.Bitangent.z);
		}


		indices = loader.indices;

		CE_INFO("size of verticies: {0}", vertices.size());
		CE_INFO("size of indices: {0}", indices.size());

		/*if (loader.albedo) albedo = loader.albedo;
		else CE_WARN("Failed to load default albedo texture");

		if (loader.normal)normal = loader.normal;
		else CE_WARN("Failed to load default normal texture");

		if (loader.metallic)metallic = loader.metallic;
		else CE_WARN("Failed to load default metallic texture");

		if (loader.roughness)roughness = loader.roughness;
		else CE_WARN("Failed to load default roughness texture");

		if (loader.ao)ao = loader.ao;
		else CE_WARN("Failed to load default ao texture");*/

		/*albedo = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_A.tga");
		normal = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_N.tga");
		metallic = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_M.tga");
		roughness = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_R.tga");
		ao = Texture2D::Create("C:\\EngineDev\\CrashEngine\\Textures\\cerberus\\cerberus_AO.tga");*/

		texCreator.reset(new TextureCreator());

		SetupMesh();
	}

	void MeshComponent::Draw(Shader* shader)
	{
		shader->Bind();


		if (albedo)     RenderCommand::BindTexture(albedo->GetRendererID(), 0);
		if (normal)     RenderCommand::BindTexture(normal->GetRendererID(), 1);
		if (metallic)   RenderCommand::BindTexture(metallic->GetRendererID(), 2);
		if (roughness)  RenderCommand::BindTexture(roughness->GetRendererID(), 3);
		if (ao)         RenderCommand::BindTexture(ao->GetRendererID(), 4);

		// draw mesh
		VA->Bind();
		RenderCommand::DrawIndexed(VA, false);
		VA->Unbind();
	}

	void MeshComponent::SetupMesh()
	{

		VA.reset(VertexArray::Create());
		IB.reset(IndexBuffer::Create(&indices[0], indices.size()));
		VB.reset(VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float)));


		BufferLayout layout1 = {
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float2, "aTexCoords"},
			{ ShaderDataType::Float3, "aNormals"},
			{ ShaderDataType::Float3, "aTangent"},
			{ ShaderDataType::Float3, "aBitangent"},
		};
		VB->SetLayout(layout1);

		VA->AddVertexBuffer(VB);

		VA->SetIndexBuffer(IB);
	}
}