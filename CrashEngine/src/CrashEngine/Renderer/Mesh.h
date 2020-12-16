#pragma once

#include "Shader.h"

#include <string>
#include <vector>

#include "Texture.h"
#include "Buffer.h"
#include "VertexArray.h"


namespace CrashEngine
{
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };



    class Mesh
    {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        // render the mesh
        void Draw(Shader* shader);

        std::vector<float>   vertices;
        std::vector<unsigned int> indices;

        std::shared_ptr <VertexArray> VA;
        std::shared_ptr <VertexBuffer> VB;
        std::shared_ptr <IndexBuffer> IB;


        // initializes all the buffer objects/arrays
        void setupMesh();

    public:
        std::shared_ptr<Texture2D> albedo;
        std::shared_ptr<Texture2D> normal;
        std::shared_ptr<Texture2D> metallic;
        std::shared_ptr<Texture2D> roughness;
        std::shared_ptr<Texture2D> ao;

    };
}
