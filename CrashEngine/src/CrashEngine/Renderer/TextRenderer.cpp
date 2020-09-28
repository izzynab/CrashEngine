#include "TextRenderer.h"
#include "cepch.h"

#include "Renderer.h"
#include "Platform/OpenGl/OpenGLTextRenderer.h"

namespace CrashEngine {

	TextRenderer* CrashEngine::TextRenderer::Create()
	{
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        case RendererAPI::API::OpenGL:  return new OpenGLTextRenderer();
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
	}
}