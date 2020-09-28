#pragma once

#include <string>

#include "Mesh.h"
#include "Shader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace CrashEngine
{
    enum TextureType
    {
        JPG, PNG, TGA
    };

    class Model
    {
    public:
        static Model* Create(std::string const& path,TextureType type,bool gamma = false);

        // draws the model
        virtual void Draw(Shader* shader) = 0;

    protected:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        virtual void loadModel(std::string const& path) = 0;

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        virtual void processNode(aiNode* node, const aiScene* scene) = 0;

        //virtual Mesh processMesh(aiMesh* mesh, const aiScene* scene) = 0;

        virtual unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma) = 0;


    public:
        // model data 
        std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::string directory;
        bool gammaCorrection;
    };

}