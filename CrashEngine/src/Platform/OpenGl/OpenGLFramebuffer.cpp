#include "cepch.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace CrashEngine {

	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec, bool createTextures)
		: m_Specification(spec)
	{
		Invalidate();
		if (createTextures)	CreateTextures();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		//glDeleteTextures(1, &m_textures[0]);
		//glDeleteTextures(1, &m_textures[1]);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::CreateTextures()
	{
		CreateTexture(0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		//CE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::CreateTexture(uint32_t id)
	{
		m_textures.push_back(0);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_textures[id]);
		glBindTexture(GL_TEXTURE_2D, m_textures[id]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, m_textures[id], 0);

	}

	void OpenGLFramebuffer::CreateMSAATexture()
	{
		m_textures.push_back(0);
		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_textures[0]);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textures[0]);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, m_Specification.Width, m_Specification.Height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_textures[0], 0);
		
		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthAttachment);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height,GL_TRUE);
		//glTexStorage2D(GL_TEXTURE_2D_MULTISAMPLE, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthAttachment, 0);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::InitializeMultipleTextures(const int amount)
	{
		std::vector<unsigned int> a;
		for (int i = 0; i < amount; i++)
		{
			a.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glDrawBuffers(amount, a.data());

	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			CE_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		//Invalidate();
		SetNewTexture(width, height);
	}

	void OpenGLFramebuffer::SetTexture(int texTarget, uint32_t textureID, int mipMapLevel, uint32_t id)
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texTarget, textureID, mipMapLevel);
	}

	void OpenGLFramebuffer::SetDepthTexture(int texTarget, uint32_t textureID)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texTarget, textureID,0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	void OpenGLFramebuffer::SetNewTexture(uint32_t width, uint32_t height)
	{
		for (int i = 0; i < m_textures.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
	}




	OpenGLRenderbuffer::OpenGLRenderbuffer()
	{
		Invalidate();
	}

	OpenGLRenderbuffer::~OpenGLRenderbuffer()
	{
		glDeleteRenderbuffers(1, &m_RendererID);
	}

	void OpenGLRenderbuffer::Invalidate()
	{
		//glCreateRenderbuffers(1, &m_RendererID);
		glGenRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	}

	void OpenGLRenderbuffer::Bind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	}

	void OpenGLRenderbuffer::Unbind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void OpenGLRenderbuffer::AttachToFramebuffer()
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RendererID);
	}

	void OpenGLRenderbuffer::SetStorage(int format, int width, int height)
	{
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	}

	/*void OpenGLRenderbuffer::SetMSAAStorage(int samples, int width, int height)
	{
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	}*/

}
