#include "cepch.h"
#include "SSAO.h"
#include "CrashEngine/Core/Application.h"

namespace CrashEngine
{
	SSAO::SSAO()
	{
		quad.reset(new Quad());

		ssaoShader = Shader::Create("basic.vert", "ssao.frag");
		ssaoShader->Bind();
		ssaoShader->SetUniformInt("gPosition", 0);
		ssaoShader->SetUniformInt("gNormal", 1);
		ssaoShader->SetUniformInt("texNoise", 2);

		ssaoBlurShader = Shader::Create("basic.vert", "ssaoBlur.frag");
		ssaoBlurShader->Bind();
		ssaoBlurShader->SetUniformInt("scene", 0);


		//sample kernel
		std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		for (unsigned int i = 0; i < kernelSize; ++i)
		{
			glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			float scale = float(i) / kernelSize;

			// scale samples s.t. they're more aligned to center of kernel
			scale = 0.1f + scale * scale * (1.0f - 0.1f);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		// generate noise texture
		std::vector<glm::vec3> ssaoNoise;
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
			ssaoNoise.push_back(noise);
		}

		TextureSpecification texspec;
		texspec.Width = 4;
		texspec.Height = 4;
		texspec.DataFormat = DataFormat::RGB;
		texspec.MagFilterParam = FilterParam::NEAREST;
		texspec.MinFilterParam = FilterParam::NEAREST;
		texspec.internalFormat = InternalFormat::RGBA16F;
		texspec.type = Type::FLOAT;
		texspec.WrapParam = WrapParam::REPEAT;
		noiseTexture = Texture2D::Create(texspec);
		noiseTexture->SetData(&ssaoNoise[0]);
	}

	void SSAO::Render(float width, float height,uint32_t GBufferPosition, uint32_t GBufferNormals, uint32_t viewID)
	{
		std::shared_ptr<Framebuffer> ssaoFramebuffer = views[viewID].ssaoFramebuffer;
		std::shared_ptr<Framebuffer> ssaoBlurFramebuffer = views[viewID].ssaoBlurFramebuffer;

		noiseScale = glm::vec2(width / 4.0, height / 4.0);

		ssaoFramebuffer->Resize(width, height);
		ssaoBlurFramebuffer->Resize(width, height);

		ssaoFramebuffer->Bind();
		RenderCommand::Clear();
		ssaoShader->Bind();
		RenderCommand::BindTexture(GBufferPosition, 0);//position
		RenderCommand::BindTexture(GBufferNormals, 1);//normal
		RenderCommand::BindTexture(noiseTexture->GetRendererID(), 2);//noise
		for (unsigned int i = 0; i < kernelSize; ++i)
			ssaoShader->SetUniformVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
		ssaoShader->SetUniformInt("ssaoON", ssao);
		ssaoShader->SetUniformInt("kernelSize", kernelSize);
		ssaoShader->SetUniformFloat("radius", radius);
		ssaoShader->SetUniformFloat("bias", bias);
		ssaoShader->SetUniformFloat("power", power);
		ssaoShader->SetUniformVec2("noiseScale", noiseScale);
		quad->RenderQuad();
		ssaoFramebuffer->Unbind();

		ssaoBlurFramebuffer->Bind();
		RenderCommand::Clear();
		ssaoBlurShader->Bind();
		RenderCommand::BindTexture(ssaoFramebuffer->GetColorAttachmentRendererID(0), 0);
		quad->RenderQuad();
		ssaoBlurFramebuffer->Unbind();
	}

	void SSAO::AddView(float width, float height, uint32_t id)
	{
		FramebufferSpecification spec;
		spec.Height = height;
		spec.Width = width;

		view view;
		view.id = id;

		view.ssaoFramebuffer = Framebuffer::Create(spec);
		view.ssaoBlurFramebuffer = Framebuffer::Create(spec);

		views.push_back(view);
	}
}