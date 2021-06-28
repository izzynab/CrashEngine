#include "cepch.h"
#include "Mesh.h"

#include "CrashEngine/Renderer/Renderer.h"

namespace CrashEngine {

	Mesh::Mesh(std::string const& path)
        :path(path)
	{
        LoadMesh(path);
        //material = new Material("default_material");
        material.reset(new Material("default_material"));
        
	}

    Mesh::Mesh(std::string const& path, std::shared_ptr<Material>& mat)
        :path(path)
    {
        LoadMesh(path);
        //material = new Material("default_material");
        material = mat;

    }

	void Mesh::LoadMesh(std::string const& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			CE_CORE_ERROR("Assimp: {0} ", importer.GetErrorString());
			return;
		}

		directory = path.substr(0, path.find_last_of("\\"));

		processNode(scene->mRootNode, scene);	

	}

	void Mesh::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

    ChildMesh Mesh::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; 

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;

                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
       
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }


       /*aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        std::shared_ptr<Material> material;
        material.reset(new Material);

        material->albedo = LoadTexture(mat, aiTextureType_BASE_COLOR);
        material->metallic = LoadTexture(mat, aiTextureType_METALNESS);
        material->normal = LoadTexture(mat, aiTextureType_NORMAL_CAMERA);
        material->roughness = LoadTexture(mat, aiTextureType_DIFFUSE_ROUGHNESS);
        material->ao = LoadTexture(mat, aiTextureType_AMBIENT_OCCLUSION);*/

        // return a mesh object created from the extracted mesh data
        return ChildMesh(vertices, indices);
    }

    std::shared_ptr<Texture2D> Mesh::LoadTexture(aiMaterial* mat, aiTextureType texturetype)
    {
        aiString str;
        mat->GetTexture(texturetype, 0, &str);
        if (str.C_Str() < "   ") return NULL;
        std::string filename = std::string(str.C_Str());
        filename = this->directory + '/' + filename;
        std::shared_ptr<Texture2D> texture = Texture2D::Create(filename);

        CE_CORE_INFO("{0}", filename);

        return texture;
    }

    


	void Mesh::Draw(Shader* shader,bool bindTextures)
	{
        if (bindTextures)
        {
            RenderCommand::BindTexture(material->albedo->GetRendererID(), 0);
            RenderCommand::BindTexture(material->normal->GetRendererID(), 1);
            RenderCommand::BindTexture(material->metallic->GetRendererID(), 2);
            RenderCommand::BindTexture(material->roughness->GetRendererID(), 3);
            RenderCommand::BindTexture(material->ao->GetRendererID(), 4);
        }
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}



	ChildMesh::ChildMesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices)
	{
        for (Vertex i : Vertices)
        {
            this->vertices.push_back(i.Position.x);
            this->vertices.push_back(i.Position.y);
            this->vertices.push_back(i.Position.z);

            this->vertices.push_back(i.TexCoords.x);
            this->vertices.push_back(i.TexCoords.y);

            this->vertices.push_back(i.Normal.x);
            this->vertices.push_back(i.Normal.y);
            this->vertices.push_back(i.Normal.z);

            this->vertices.push_back(i.Tangent.x);
            this->vertices.push_back(i.Tangent.y);
            this->vertices.push_back(i.Tangent.z);

            this->vertices.push_back(i.Bitangent.x);
            this->vertices.push_back(i.Bitangent.y);
            this->vertices.push_back(i.Bitangent.z);
        }

		this->indices = Indices;

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

	void ChildMesh::Draw(Shader* shader)
	{
		shader->Bind();

		// draw mesh
		VA->Bind();
		RenderCommand::DrawIndexed(VA, false);
		VA->Unbind();
	}
}