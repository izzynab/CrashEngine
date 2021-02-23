#pragma once
#include <memory>

namespace CrashEngine{

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		// FramebufferFormat Format = 
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	enum class Color
	{
		RG, RGB, RGBA
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void CreateTextures() = 0;
		virtual void CreateTexture(uint32_t id, Color color = Color::RGBA) = 0;
		virtual void InitializeMultipleTextures(const int amount) = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void SetTexture(int texTarget, uint32_t textureID, int mipMapLevel, uint32_t id = 0) = 0;
		virtual void SetDepthTexture(int texTarget, uint32_t textureID) = 0;
		virtual void SetNewTexture(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t id) = 0;
		virtual uint32_t GetDepthAttachmentRenderID() = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual inline uint32_t GetRenderID() = 0;

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec,bool createTetures = true);

	};

	class MSAAFramebuffer
	{
	public:
		virtual ~MSAAFramebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void BlitToFramebuffer(std::shared_ptr<Framebuffer> framebuffer) = 0;

		virtual void CreateMSAATexture(uint32_t id, Color color) = 0;
		virtual void CreateMSAATextures(int msaa_value = 4) = 0;
		virtual void InitializeMultipleTextures(const int amount) = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void SetTexture(int texTarget, uint32_t textureID, int mipMapLevel, uint32_t id = 0) = 0;
		virtual void SetDepthTexture(int texTarget, uint32_t textureID) = 0;
		virtual void SetNewTexture(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t id) = 0;
		virtual uint32_t GetDepthAttachmentRenderID() = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;
		virtual inline uint32_t GetRenderID() = 0;

		static std::shared_ptr<MSAAFramebuffer> Create(const FramebufferSpecification & spec);

	};

	class Renderbuffer
	{
	public:
		virtual ~Renderbuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		
		virtual void AttachToFramebuffer() = 0;

		virtual void SetStorage(int format, int width, int height) = 0;
		//virtual void SetMSAAStorage(int samples, int width, int height) = 0;

		static std::shared_ptr<Renderbuffer> Create();
	};
}
