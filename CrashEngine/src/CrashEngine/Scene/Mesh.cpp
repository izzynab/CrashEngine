#include "cepch.h"
#include "Mesh.h"

#include "CrashEngine/Renderer/Renderer.h"

namespace CrashEngine {

	Mesh::Mesh(std::string const& path)
		: directory(path)
	{
		LoadMesh(path);
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

		directory = path.substr(0, path.find_last_of('/'));

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
        std::vector<std::shared_ptr<Texture2D>> textures;

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


        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        std::vector<std::shared_ptr<Texture2D>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<std::shared_ptr<Texture2D>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<std::shared_ptr<Texture2D>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<std::shared_ptr<Texture2D>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return ChildMesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<std::shared_ptr<Texture2D>> Mesh::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<std::shared_ptr<Texture2D>> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j]->m_Path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                std::string filename = std::string(str.C_Str());
                filename = this->directory + '/' + filename;
                std::shared_ptr<Texture2D> texture = Texture2D::Create(filename);

                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }


	void Mesh::Draw(Shader* shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}



	ChildMesh::ChildMesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<std::shared_ptr<Texture2D>> Textures)
	{
		//this->vertices = vertices;

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
		this->textures = Textures;

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
		//if (albedo)     RenderCommand::BindTexture(albedo->GetRendererID(), 0);
		//if (normal)     RenderCommand::BindTexture(normal->GetRendererID(), 1);
		//if (metallic)   RenderCommand::BindTexture(metallic->GetRendererID(), 2);
		//if (roughness)  RenderCommand::BindTexture(roughness->GetRendererID(), 3);
		//if (ao)         RenderCommand::BindTexture(ao->GetRendererID(), 4);

		// draw mesh
		VA->Bind();
		RenderCommand::DrawIndexed(VA, false);
		VA->Unbind();
	}
}