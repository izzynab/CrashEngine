#include "cepch.h"
#include "MeshLoader.h"

namespace CrashEngine {

    MeshLoader::MeshLoader(std::string const& path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            CE_CORE_ERROR("ERROR::ASSIMP:: {}", importer.GetErrorString());
            return;
        }

        if (scene->mRootNode->mNumMeshes > 1) CE_WARN("Engine doesn't support loading multiple meshes in one model");
        aiMesh* mesh = scene->mMeshes[0];

        loadMeshData(mesh, scene);
    }


    void MeshLoader::loadMeshData(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            Vertices.push_back(vertex);
        }
        // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                Indices.push_back(face.mIndices[j]);
        }

        vertices = Vertices;
        indices = Indices;



        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        albedo = LoadTexture(material, aiTextureType_BASE_COLOR);
        normal = LoadTexture(material, aiTextureType_NORMAL_CAMERA);
        metallic = LoadTexture(material, aiTextureType_METALNESS);
        roughness = LoadTexture(material, aiTextureType_DIFFUSE_ROUGHNESS);
        ao = LoadTexture(material, aiTextureType_AMBIENT_OCCLUSION);


    }



    std::shared_ptr<Texture2D> MeshLoader::LoadTexture(aiMaterial* mat, aiTextureType type)
    {
        std::shared_ptr<Texture2D> texture;

        aiString str;
        mat->GetTexture(type, 0, &str);

        texture = Texture2D::Create(str.C_Str());

        CE_INFO("Directory of texture: {0}", str.C_Str());

        return texture;
    }

}