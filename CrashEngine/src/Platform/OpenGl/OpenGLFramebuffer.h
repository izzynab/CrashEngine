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

		virtual void BlitDepthToFramebuffer(std::shared_ptr<Framebuffer> framebuffer);

		virtual void CreateTextures();
		virtual void CreateTexture(uint32_t id, Color color = Color::RGBA);
		virtual void InitializeMultipleTextures(const int amount);

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void SetTexture(int texTarget, uint32_t textureID, int mipMapLevel,uint32_t id = 0 ) override;
		virtual void SetDepthTexture(int texTarget, uint32_t textureID) override;
		virtual void SetNewTexture(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_textures[0]; }
		virtual uint32_t GetColorAttachmentRendererID(uint32_t id) override { return m_textures[id]; }
		virtual uint32_t GetDepthAttachmentRenderID() override { return m_DepthAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		virtual inline uint32_t GetRenderID() override { return m_RendererID; }


	private:
		uint32_t m_RendererID = 0;
		std::vector<uint32_t> m_textures;
		uint32_t m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
		int msaa;
	};

	class OpenGLMSAAFramebuffer : public MSAAFramebuffer
	{
	public:
		OpenGLMSAAFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLMSAAFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void BlitToFramebuffer(std::shared_ptr<Framebuffer> framebuffer);

		virtual void CreateMSAATexture(uint32_t id, Color color);
		virtual void CreateMSAATextures(int msaa_value);
		virtual void InitializeMultipleTextures(const int amount);

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void SetTexture(int texTarget, uint32_t textureID, int mipMapLevel, uint32_t id = 0) override;
		virtual void SetDepthTexture(int texTarget, uint32_t textureID) override;
		virtual void SetNewTexture(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_textures[0]; }
		virtual uint32_t GetColorAttachmentRendererID(uint32_t id) override { return m_textures[id]; }
		virtual uint32_t GetDepthAttachmentRenderID() override { return m_DepthAttachment; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		virtual inline uint32_t GetRenderID() override { return m_RendererID; }

	private:
		uint32_t m_RendererID = 0;
		std::vector<uint32_t> m_textures;
		uint32_t m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
		int msaa;
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
		//virtual void SetMSAAStorage(int samples, int width, int height) override;

	private:
		uint32_t m_RendererID = 0;
	};
}