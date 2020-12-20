#include "cepch.h"
#include "Mesh.h"
#include "Renderer.h"

#include "glad/glad.h"
#include "Texture.h"

namespace CrashEngine
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
        for (Vertex i : vertices)
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


		this->indices = indices;


		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();

	}

    void Mesh::Draw(Shader* shader)
    {
        shader->Bind();

        if(albedo)      RenderCommand::BindTexture(albedo->GetRendererID(), 0);
        if (normal)     RenderCommand::BindTexture(normal->GetRendererID(), 1);
        if (metallic)   RenderCommand::BindTexture(metallic->GetRendererID(), 2);
        if (roughness)  RenderCommand::BindTexture(roughness->GetRendererID(), 3);
        if (ao)         RenderCommand::BindTexture(ao->GetRendererID(), 4);


        // draw mesh
        VA->Bind();
        RenderCommand::DrawIndexed(VA, false);
        VA->Unbind();

    }

	void Mesh::setupMesh()
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