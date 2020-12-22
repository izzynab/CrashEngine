#include "cepch.h"
#include "TextureCreator.h"

#include "CrashEngine/Core/Defines.h"
#include "RenderCommand.h"

namespace CrashEngine {

	TextureCreator::TextureCreator()
	{
		defaultShader = Shader::Create("Basic.vert", "Basic.frag");

		FramebufferSpecification spec;
		spec.Height = 512;
		spec.Width = 512;

		framebuffer = Framebuffer::Create(spec);

		framebuffer = Framebuffer::Create(spec);
		framebuffer->Bind();


		renderbuffer = Renderbuffer::Create();
		renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, 512, 512);
		renderbuffer->AttachToFramebuffer();

		quad.reset(new Quad());

	}


	std::shared_ptr<Texture2D> TextureCreator::CreateTexture(float Width, float Height, Shader* shader)
	{
		// pbr: generate a 2D LUT from the BRDF equations used.
		std::shared_ptr<Texture2D> Texture = Texture2D::Create(Width, Height);

		// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
		framebuffer->Bind();
		renderbuffer->Bind();
		renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, Width, Height);
		framebuffer->SetTexture(CE_TEXTURE_2D, Texture->GetRendererID(), 0);

		RenderCommand::SetViewport(Width, Height);
		shader->Bind();
		RenderCommand::Clear();
		quad->RenderQuad();

		framebuffer->Unbind();

		return Texture;
	}
	std::shared_ptr<Texture2D> TextureCreator::CreateTexture(float Width, float Height, glm::vec3 color)
	{
		// pbr: generate a 2D LUT from the BRDF equations used.
		std::shared_ptr<Texture2D> Texture = Texture2D::Create(Width, Height);

		// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
		framebuffer->Bind();
		renderbuffer->Bind();
		renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, Width, Height);
		framebuffer->SetTexture(CE_TEXTURE_2D, Texture->GetRendererID(), 0);


		RenderCommand::SetViewport(Width, Height);
		defaultShader->Bind();
		defaultShader->SetUniformVec3("color", color);
		RenderCommand::Clear();
		quad->RenderQuad();

		framebuffer->Unbind();

		return Texture;
	}
}