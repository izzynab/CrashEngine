#pragma once

#include <string>

#include "Mesh.h"
#include "Shader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


namespace CrashEngine
{
    class Model
    {
    public:
        Model(std::string const& path,bool gamma = false);

        // draws the model
        void Draw(Shader* shader);

    protected:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(std::string const& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);


    public:
        // model data 
        std::string directory;
        bool gammaCorrection;

        std::vector<Mesh>  meshes;
    };

}