#pragma once

#include "CrashEngine/Renderer/Mesh.h"


namespace CrashEngine
{
    class OpenGLMesh : public Mesh
    {
    public:
        // constructor
        OpenGLMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures);

        // render the mesh
        virtual void Draw(Shader* shader) override;


    private:
        // render data 
        unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh();

    };
}
