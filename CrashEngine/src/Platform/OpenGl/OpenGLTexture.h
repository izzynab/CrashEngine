#pragma once

#include "CrashEngine/Renderer/Texture.h"

#include <glad/glad.h>

namespace CrashEngine {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(TextureSpecification spec);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return specification.Width; }
		virtual uint32_t GetHeight() const override { return specification.Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data) override;

		virtual void CreateMipmap() override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}
	private:
		TextureSpecification specification;

		uint32_t m_RendererID;
	};

	class OpenGLDepthTexture : public DepthTexture
	{
	public:
		OpenGLDepthTexture(uint32_t width, uint32_t height);
		virtual ~OpenGLDepthTexture();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data) {};

		virtual void CreateMipmap();

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLDepthTexture&)other).m_RendererID;
		}
	private:
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};

	class OpenGLTextureHDR : public TextureHDR
	{
	public:
		OpenGLTextureHDR(const std::string& path);
		virtual ~OpenGLTextureHDR();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data) override;

		virtual void CreateMipmap() override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTextureHDR&)other).m_RendererID;
		}
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

	class OpenGLCubemapTexture : public CubemapTexture
	{
	public:
		OpenGLCubemapTexture(uint32_t width, uint32_t height, bool mipmap);
		virtual ~OpenGLCubemapTexture();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data) override;

		virtual void CreateMipmap() override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLCubemapTexture&)other).m_RendererID;
		}
	private:
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}