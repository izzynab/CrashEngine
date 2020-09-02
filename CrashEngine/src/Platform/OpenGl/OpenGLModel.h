#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <string>

#include "CrashEngine/Renderer/Model.h"
#include "OpenGLMesh.h"

namespace CrashEngine
{

    class OpenGLModel : public Model
    {
    public:
        // constructor, expects a filepath to a 3D model.
        OpenGLModel(std::string const& path, bool gamma);

        // draws the model, and thus all its meshes
        virtual void Draw(Shader* shader) override;

    protected:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        virtual void loadModel(std::string const& path) override;

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        virtual void processNode(aiNode* node, const aiScene* scene) override;

        OpenGLMesh processMesh(aiMesh* mesh, const aiScene* scene);

        virtual unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma) override;

        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    public:
        std::vector<OpenGLMesh>  meshes;

    };

}