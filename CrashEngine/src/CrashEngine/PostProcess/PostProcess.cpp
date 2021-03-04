#include "cepch.h"
#include "PostProcess.h"
#include "CrashEngine/Renderer/Renderer.h"

namespace CrashEngine {

	PostProcess::PostProcess()
	{
		quad.reset(new Quad());

		FramebufferSpecification spec;
		spec.Height = 1080;
		spec.Width = 2560;

		//----------------HDR and Gamma-------------------------
		HDRGammaCorrection = Shader::Create("Basic.vert", "GammaHDR.frag");

		HDRGammaCorrection->Bind();
		HDRGammaCorrection->SetUniformInt("scene", 0);

		//----------------blur-------------------------
		blurFramebuffer[0] = Framebuffer::Create(spec);
		blurFramebuffer[1] = Framebuffer::Create(spec);

		bloom = Framebuffer::Create(spec);
		draw_framebuffer = Framebuffer::Create(spec);

		blurShader = Shader::Create("Basic.vert", "blur.frag");
		BloomMixShader = Shader::Create("Basic.vert", "mixbloom.frag");
		brightShader = Shader::Create("Basic.vert", "brightness.frag");

		brightShader->Bind();
		brightShader->SetUniformInt("frame_texture", 0);

		BloomMixShader->Bind();
		BloomMixShader->SetUniformInt("scene", 0);
		BloomMixShader->SetUniformInt("bloomBlur", 1);

		//----------------FXAA-------------------------
		FXAAShader = Shader::Create("Basic.vert", "fxaa.frag");

		FXAAShader->Bind();
		FXAAShader->SetUniformInt("colorTexture", 0);
	}

	void PostProcess::Blur(std::shared_ptr<Framebuffer>& framebuffer)
	{
		float width = framebuffer->GetSpecification().Width;
		float height = framebuffer->GetSpecification().Height;
		blurFramebuffer[0]->Resize(width, height);
		blurFramebuffer[1]->Resize(width, height);

		bloom->Resize(width, height);
		draw_framebuffer->Resize(width, height);

		RenderCommand::BlitFramebuffers(framebuffer, draw_framebuffer);


		bloom->Bind();
		RenderCommand::Clear();
		brightShader->Bind();
		RenderCommand::BindTexture(draw_framebuffer->GetColorAttachmentRendererID(), 0);
		quad->RenderQuad();
		brightShader->Unbind();
		bloom->Unbind();

		// 2. blur bright fragments with two-pass Gaussian Blur 
		bool horizontal = true, first_iteration = true;
		unsigned int amount = 10;
		blurShader->Bind();
		for (unsigned int i = 0; i < amount; i++)
		{
			blurFramebuffer[horizontal]->Bind();
			blurFramebuffer[horizontal]->SetNewTexture(draw_framebuffer->GetSpecification().Width, draw_framebuffer->GetSpecification().Height);

			RenderCommand::Clear();
			blurShader->SetUniformInt("horizontal", horizontal);

			RenderCommand::BindTexture(first_iteration ? bloom->GetColorAttachmentRendererID() : blurFramebuffer[!horizontal]->GetColorAttachmentRendererID(), 0);
			quad->RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
			blurFramebuffer[horizontal]->Unbind();
		}


		// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		//RenderCommand::Clear();

		framebuffer->Bind();
		RenderCommand::Clear();
		BloomMixShader->Bind();
		RenderCommand::BindTexture(draw_framebuffer->GetColorAttachmentRendererID(), 0);
		RenderCommand::BindTexture(blurFramebuffer[1]->GetColorAttachmentRendererID(), 1);
		BloomMixShader->SetUniformInt("exposure", exposure);
		BloomMixShader->SetUniformInt("blur", blur);

		quad->RenderQuad();

		framebuffer->Unbind();
	}

	void PostProcess::GammaHDRCorretion(std::shared_ptr<Framebuffer>& framebuffer)
	{
		draw_framebuffer->Resize(framebuffer->GetSpecification().Width, framebuffer->GetSpecification().Height);
		RenderCommand::BlitFramebuffers(framebuffer, draw_framebuffer);

		framebuffer->Bind();
		RenderCommand::Clear();
		HDRGammaCorrection->Bind();
		RenderCommand::BindTexture(draw_framebuffer->GetColorAttachmentRendererID(), 0);
		HDRGammaCorrection->SetUniformInt("exposure", exposure);
		quad->RenderQuad();

		framebuffer->Unbind();

	}

	void PostProcess::ApplyFXAA(std::shared_ptr<Framebuffer>& framebuffer)
	{
		draw_framebuffer->Resize(framebuffer->GetSpecification().Width, framebuffer->GetSpecification().Height);
		RenderCommand::BlitFramebuffers(framebuffer, draw_framebuffer);

		framebuffer->Bind();
		RenderCommand::Clear();
		FXAAShader->Bind();
		glm::vec2 vector = glm::vec2(1 / float(framebuffer->GetSpecification().Width), 1 / float(framebuffer->GetSpecification().Height));
		FXAAShader->SetUniformVec2("inverseScreenSize", vector);
		CE_CORE_INFO("{0},{1}", vector.x, vector.y);
		RenderCommand::BindTexture(draw_framebuffer->GetColorAttachmentRendererID(), 0);
		quad->RenderQuad();

		framebuffer->Unbind();
	}
}