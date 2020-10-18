#include "cepch.h"
#include "BasicShapes.h"

#include "CrashEngine/Renderer/Renderer.h"

namespace CrashEngine {

    Sphere::Sphere()
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<uint32_t> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }

        SphereVA.reset(VertexArray::Create());
        SphereIB.reset(IndexBuffer::Create(&indices[0], indices.size()));

        std::shared_ptr<VertexBuffer> SphereVB;
        SphereVB.reset(VertexBuffer::Create(&data[0], data.size() * sizeof(float)));


        BufferLayout layout1 = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoords"},
            { ShaderDataType::Float3, "a_Normals"}
        };
        SphereVB->SetLayout(layout1);

        SphereVA->AddVertexBuffer(SphereVB);

        SphereVA->SetIndexBuffer(SphereIB);

    }


    void Sphere::RenderSphere()
    {
        SphereVA->Bind();
        Renderer::Submit(SphereVA,true);
    }


    Square::Square()
    {
        SquareVA.reset(VertexArray::Create());
       // SquareIB.reset(IndexBuffer::Create(&indices[0], indices.size()));

        std::shared_ptr<VertexBuffer> SquareVB;
        SquareVB.reset(VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float)));

        BufferLayout layout1 = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoords"},
            { ShaderDataType::Float3, "a_Normals"},
        };
        SquareVB->SetLayout(layout1);

        SquareVA->AddVertexBuffer(SquareVB);
        //SquareVA->SetIndexBuffer(SquareIB);
        
    }


    void Square::RenderSquare()
    {
        SquareVA->Bind();
        Renderer::SubmitDebug(SquareVA,36);
    }


    Quad::Quad()
    {
        QuadVA.reset(VertexArray::Create());
        //SquareIB.reset(IndexBuffer::Create(&indices[0], indices.size()));

        std::shared_ptr<VertexBuffer> QuadVB;
        QuadVB.reset(VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float)));

        BufferLayout layout1 = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoords"}
        };
        QuadVB->SetLayout(layout1);

        QuadVA->AddVertexBuffer(QuadVB);

    }

    void Quad::RenderQuad()
    {
        QuadVA->Bind();
        Renderer::SubmitDebug(QuadVA,6);
    }
}
