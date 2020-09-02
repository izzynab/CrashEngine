#include "cepch.h"
#include "Mesh.h"

#include "Renderer.h"

#include "Platform/OpenGl/OpenGLMesh.h"

namespace CrashEngine
{
    Mesh* CrashEngine::Mesh::Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        case RendererAPI::API::OpenGL:  return new OpenGLMesh(vertices, indices, textures);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    void Mesh::Draw(Shader* shader)
    {
    }
}