#include "cepch.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace CrashEngine {

	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec, bool createTextures )
		: m_Specification(spec)
	{
		Invalidate();
		if (createTextures)	CreateTextures();
		else
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
		}
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

	void OpenGLFramebuffer::BlitDepthToFramebuffer(std::shared_ptr<Framebuffer> framebuffer)
	{
		Bind();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->GetRenderID());
	
		glReadBuffer(GL_DEPTH_ATTACHMENT);
		glDrawBuffer(GL_DEPTH_ATTACHMENT);
		glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, framebuffer->GetSpecification().Width, framebuffer->GetSpecification().Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		Unbind();
	}

	void OpenGLFramebuffer::CreateTextures()
	{
		CreateTexture(0,Color::RGBA);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		//CE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::CreateTexture(uint32_t id, Color color)
	{
		m_textures.push_back(id);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_textures[id]);
		glBindTexture(GL_TEXTURE_2D, m_textures[id]);

		/*switch (color)
		{
		case Color::RG:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RG, GL_UNSIGNED_BYTE, nullptr);
		case Color::RGB:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		case Color::RGBA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, m_textures[id], 0);

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
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		m_DepthAttachment = textureID;
	}

	void OpenGLFramebuffer::SetNewTexture(uint32_t width, uint32_t height)
	{
		for (int i = 0; i < m_textures.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		}
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------
	OpenGLMSAAFramebuffer::OpenGLMSAAFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		Invalidate();

	}

	OpenGLMSAAFramebuffer::~OpenGLMSAAFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		//glDeleteTextures(1, &m_textures[0]);
		//glDeleteTextures(1, &m_textures[1]);
	}

	void OpenGLMSAAFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	}

	void OpenGLMSAAFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLMSAAFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLMSAAFramebuffer::InitializeMultipleTextures(const int amount)
	{
		std::vector<unsigned int> a;
		for (int i = 0; i < amount; i++)
		{
			a.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glDrawBuffers(amount, a.data());

	}

	void OpenGLMSAAFramebuffer::Resize(uint32_t width, uint32_t height)
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

	void OpenGLMSAAFramebuffer::SetTexture(int texTarget, uint32_t textureID, int mipMapLevel, uint32_t id)
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texTarget, textureID, mipMapLevel);
	}

	void OpenGLMSAAFramebuffer::SetDepthTexture(int texTarget, uint32_t textureID)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texTarget, textureID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	void OpenGLMSAAFramebuffer::SetNewTexture(uint32_t width, uint32_t height)
	{
		for (int i = 0; i < m_textures.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textures[i]);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA16F, m_Specification.Width, m_Specification.Height, GL_TRUE);
		}
	}

	void OpenGLMSAAFramebuffer::BlitToFramebuffer(std::shared_ptr<Framebuffer> framebuffer)
	{
		Bind();
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->GetRenderID());

		for (int i = 0; i < 4; i++)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
			glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, framebuffer->GetSpecification().Width, framebuffer->GetSpecification().Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}

		glReadBuffer(GL_DEPTH_ATTACHMENT);
		glDrawBuffer(GL_DEPTH_ATTACHMENT);
		glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, framebuffer->GetSpecification().Width, framebuffer->GetSpecification().Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		Unbind();
	}

	void OpenGLMSAAFramebuffer::CreateMSAATexture(uint32_t id, Color color)
	{
		m_textures.push_back(id);
		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_textures[id]);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textures[id]);
		/*switch (color)
		{
		case Color::RG:
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RG, m_Specification.Width, m_Specification.Height, GL_TRUE);
		case Color::RGB:
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGB, m_Specification.Width, m_Specification.Height, GL_TRUE);
		case Color::RGBA:
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA, m_Specification.Width, m_Specification.Height, GL_TRUE);
		}*/
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGBA16F, m_Specification.Width, m_Specification.Height, GL_TRUE);
		
		
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D_MULTISAMPLE, m_textures[id], 0);

	}

	void OpenGLMSAAFramebuffer::CreateMSAATextures(int msaa_value)
	{
		msaa = msaa_value;

		CreateMSAATexture(0,Color::RGB);//position
		CreateMSAATexture(1, Color::RGB);//normal
		CreateMSAATexture(2, Color::RGB);//albedo
		CreateMSAATexture(3, Color::RGB);//metallic roughness ao
		InitializeMultipleTextures(4);

		glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthAttachment);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa_value, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, GL_TRUE);
		//glTexStorage2D(GL_TEXTURE_2D_MULTISAMPLE, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_DepthAttachment, 0);



		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//---------------------------------------------------------------------------------------------------------------------------------------------------

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


}
