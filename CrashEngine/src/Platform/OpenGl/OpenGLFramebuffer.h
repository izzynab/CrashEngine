#pragma once

#include "CrashEngine/Renderer/Framebuffer.h"

namespace CrashEngine
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec,bool createTextures = true);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void CreateTextures();
		virtual void CreateTexture(uint32_t id);

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void SetTexture(int texTarget, uint32_t textureID, int mipMapLevel,uint32_t id = 0 ) override;
		virtual void SetDepthTexture(int texTarget, uint32_t textureID) override;
		virtual void SetNewTexture(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_textures[0]; }
		virtual uint32_t GetDepthAttachmentRendererID() const override { return m_textures[1]; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		std::vector<uint32_t> m_textures;
		//uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
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