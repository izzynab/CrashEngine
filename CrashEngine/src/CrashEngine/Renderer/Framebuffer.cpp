#include "cepch.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLFramebuffer.h"

namespace CrashEngine {

	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec, bool createTetures )
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(spec, createTetures);
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<MSAAFramebuffer> MSAAFramebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLMSAAFramebuffer>(spec);
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Renderbuffer> Renderbuffer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLRenderbuffer>();
		}

		CE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}