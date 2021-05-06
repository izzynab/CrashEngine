#include "cepch.h"
#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace CrashEngine {

	OpenGLTexture2D::OpenGLTexture2D(TextureSpecification spec)
	{
		specification = spec;

		//glGenTextures(1, &m_RendererID);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		// pre-allocate enough memory for the LUT texture.
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, specification.internalFormat, specification.Width, specification.Height, 0, specification.DataFormat, specification.type, 0);
		// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, specification.WrapParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, specification.WrapParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, specification.FilterParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, specification.FilterParam);

	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
	{
		specification.internalFormat = InternalFormat::RGB16F;
		specification.DataFormat = DataFormat::RGB;

		specification.Width = width;
		specification.Height = height;

		glGenTextures(1, &m_RendererID);
		// pre-allocate enough memory for the LUT texture.
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, specification.internalFormat, width, height, 0, specification.DataFormat, specification.type, 0);
		// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		//m_Path = "C:\\EngineDev\\CrashEngine\\Textures\\" + path;
		m_Path =  path;

		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		}
		CE_CORE_ASSERT(data, "Failed to load image!");
		specification.Width = width;
		specification.Height = height;

		if (channels == 4)
		{
			specification.internalFormat = InternalFormat::RGBA8;
			specification.DataFormat = DataFormat::RGBA;
		}
		else if (channels == 3)
		{
			specification.internalFormat = InternalFormat::RGB8;
			specification.DataFormat = DataFormat::RGB;
		}


		CE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, specification.DataFormat, specification.Width, specification.Height, 0, specification.DataFormat, specification.type, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data)
	{
		//uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		//CE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, specification.Width, specification.Height, specification.DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::CreateMipmap()
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::BindImageTexture()
	{
		glBindImageTexture(0, m_RendererID, 0, GL_FALSE, 0, GL_WRITE_ONLY, specification.internalFormat);
	}


	OpenGLDepthTexture::OpenGLDepthTexture(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	}


	OpenGLDepthTexture::~OpenGLDepthTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}


	void OpenGLDepthTexture::CreateMipmap()
	{
		//glClear(GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLDepthTexture::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	OpenGLTextureHDR::OpenGLTextureHDR(const std::string& path)
	{
		//m_Path = "C:\\EngineDev\\CrashEngine\\Textures\\" + path;
		m_Path = path;

		stbi_set_flip_vertically_on_load(true);
		int width, height, nrComponents;
		float* data = stbi_loadf(m_Path.c_str(), &width, &height, &nrComponents, 0);
		
		
		m_Width = width;
		m_Height = height;

		if (data)
		{
			glGenTextures(1, &m_RendererID);
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			CE_CORE_ASSERT(data, "Failed to load HDR image!");

		}
	}

	OpenGLTextureHDR::~OpenGLTextureHDR()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureHDR::SetData(void* data)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		CE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_FLOAT, data);
	}

	void OpenGLTextureHDR::CreateMipmap()
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void OpenGLTextureHDR::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}




	OpenGLCubemapTexture::OpenGLCubemapTexture(uint32_t width, uint32_t height, bool mipmap)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGB16F;
		m_DataFormat = GL_RGB;

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		if (mipmap)
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	OpenGLCubemapTexture::~OpenGLCubemapTexture()
	{
		//glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLCubemapTexture::SetData(void* data)
	{
		//uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		//CE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		//glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLCubemapTexture::CreateMipmap()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}

	void OpenGLCubemapTexture::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}
}