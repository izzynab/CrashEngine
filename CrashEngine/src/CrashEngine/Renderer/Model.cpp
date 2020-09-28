#include "cepch.h"
#include "Model.h"

#include "Renderer.h"

#include "Platform/OpenGl/OpenGLModel.h"

namespace CrashEngine
{
    Model* CrashEngine::Model::Create(std::string const& path, TextureType type, bool gamma)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        case RendererAPI::API::OpenGL:  return new OpenGLModel(path, type, gamma);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}