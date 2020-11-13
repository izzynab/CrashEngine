#pragma once

#include "CrashEngine/Renderer/Framebuffer.h"

namespace CrashEngine
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void SetTexture(int texTarget, uint32_t textureID, int mipMapLevel) override;
		virtual void SetNewTexture(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	};

	class OpenGLRenderbuffer : public Renderbuffer
	{
	public:
		OpenGLRenderbuffer();
		virtual ~OpenGLRenderbuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void AttachToFramebuffer() override;

		virtual void SetStorage(int format, int width, int height) override; 

	private:
		uint32_t m_RendererID = 0;
	};
}