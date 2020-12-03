#include "cepch.h"
#include "Mesh.h"
#include "Renderer.h"

#include "glad/glad.h"

namespace CrashEngine
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();

	}

    void Mesh::Draw(Shader* shader)
    {
        shader->Bind();

        RenderCommand::BindTexture(albedo->GetRendererID(), 0);
        RenderCommand::BindTexture(normal->GetRendererID(), 1);
        RenderCommand::BindTexture(metallic->GetRendererID(), 2);
        RenderCommand::BindTexture(roughness->GetRendererID(), 3);
        RenderCommand::BindTexture(ao->GetRendererID(), 4);
       
        // draw mesh
        Renderer::Submit(VA, false);


    }

	void Mesh::setupMesh()
	{    
        VA.reset(VertexArray::Create());
        IB.reset(IndexBuffer::Create(&indices[0], indices.size()));

        //VB.reset(VertexBuffer::Create(&data[0], data.size() * sizeof(float)));
        VB.reset(VertexBuffer::Create(&vertices[0].Position.x, vertices.size() * sizeof(Vertex)));

        BufferLayout layout1 = {
            { ShaderDataType::Float3, "aPosition" },
            { ShaderDataType::Float3, "aNormals"},
            { ShaderDataType::Float2, "aTexCoords"},
            { ShaderDataType::Float3, "aTangent"},
            { ShaderDataType::Float3, "aBitangent"},
        };
        VB->SetLayout(layout1);

        VA->AddVertexBuffer(VB);
        VA->SetIndexBuffer(IB);
        VA->Unbind();
       
	}
}