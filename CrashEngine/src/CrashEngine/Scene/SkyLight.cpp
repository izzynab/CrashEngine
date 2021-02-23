#include "cepch.h"

#include "SkyLight.h"
#include "CrashEngine/Core/Defines.h"

namespace CrashEngine {

	SkyLight::SkyLight()
	{
		cube.reset(new Cube());
		quad.reset(new Quad());

		equirectangularToCubemapShader = Shader::Create("cubemap.vert", "equirectangularToCubemap.frag");
		equirectangularToCubemapShader->Bind();
		equirectangularToCubemapShader->SetUniformInt("equirectangularMap", 0);
		equirectangularToCubemapShader->SetUniformMat4("projection", captureProjection);

		irradianceShader = Shader::Create("cubemap.vert", "irradiance.frag");
		irradianceShader->Bind();
		irradianceShader->SetUniformInt("environmentMap", 0);
		irradianceShader->SetUniformMat4("projection", captureProjection);

		prefilterShader = Shader::Create("cubemap.vert", "prefilter.frag");
		prefilterShader->Bind();
		prefilterShader->SetUniformInt("environmentMap", 0);
		prefilterShader->SetUniformMat4("projection", captureProjection);

		brdfShader = Shader::Create("Basic.vert", "brdf.frag");

		backgroundShader = Shader::Create("background.vert", "background.frag");
		backgroundShader->Bind();
		backgroundShader->SetUniformInt("environmentMap", 0);

		FramebufferSpecification spec;
		spec.Height = 1080;
		spec.Width = 1080;

		Framebuffer = Framebuffer::Create(spec);
		Framebuffer->Bind();

		Renderbuffer = Renderbuffer::Create();
		Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, 1080, 1080);
		Renderbuffer->AttachToFramebuffer();

		// set default settings
		RenderCommand::Enable(CE_DEPTH_TEST);
		RenderCommand::Enable(CE_TEXTURE_CUBE_MAP_SEAMLESS);
		RenderCommand::DepthFunc(CE_LEQUAL);


		// pbr: setup cubemap to render to and attach to framebuffer
		Cubemap = CubemapTexture::Create(1080, 1080, true);

		// pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
		Irradiancemap = CubemapTexture::Create(32, 32, false);

		// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
		Prefiltermap = CubemapTexture::Create(128, 128, true);
		Prefiltermap->CreateMipmap();

		texCreator.reset(new TextureCreator());
		brdfLUTTexture = texCreator->CreateTexture(512, 512, brdfShader);

		//LoadHDR("C:\\EngineDev\\CrashEngine\\Textures\\hdr\\14-Hamarikyu_Bridge_B_3k.hdr");
	}

	void SkyLight::LoadHDR(std::string path)
	{
		FramebufferSpecification spec;
		spec.Height = 1080;
		spec.Width = 1080;

		Framebuffer = Framebuffer::Create(spec);
		Framebuffer->Bind();

		directory = path;
		HDR = TextureHDR::Create(path);

		// pbr: convert HDR equirectangular environment map to cubemap equivalent
		equirectangularToCubemapShader->Bind();
		RenderCommand::BindTexture(HDR->GetRendererID(), 0);

		RenderCommand::SetViewport(1080, 1080);
		Framebuffer->Bind();
		for (unsigned int i = 0; i < 6; ++i)
		{
			equirectangularToCubemapShader->SetUniformMat4("view", captureViews[i]);
			Framebuffer->SetTexture(CE_TEXTURE_CUBE_MAP_POSITIVE_X + i, Cubemap->GetRendererID(), 0);
			RenderCommand::Clear();

			cube->RenderCube();
		}
		Framebuffer->Unbind();

		// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
		Cubemap->CreateMipmap();

		Framebuffer->Bind();
		Renderbuffer->Bind();
		Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, 32, 32);


		// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
		irradianceShader->Bind();
		RenderCommand::BindCubemap(Cubemap->GetRendererID(), 0);


		RenderCommand::SetViewport(32, 32); // don't forget to configure the viewport to the capture dimensions.
		Framebuffer->Bind();
		for (unsigned int i = 0; i < 6; ++i)
		{
			irradianceShader->SetUniformMat4("view", captureViews[i]);
			Framebuffer->SetTexture(CE_TEXTURE_CUBE_MAP_POSITIVE_X + i, Irradiancemap->GetRendererID(), 0);
			RenderCommand::Clear();
			cube->RenderCube();
		}
		Framebuffer->Unbind();

		// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
		prefilterShader->Bind();
		RenderCommand::BindCubemap(Cubemap->GetRendererID(), 0);

		Framebuffer->Bind();
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);
			Renderbuffer->Bind();
			Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, mipWidth, mipHeight);
			RenderCommand::SetViewport(mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			prefilterShader->SetUniformFloat("roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				prefilterShader->SetUniformMat4("view", captureViews[i]);
				Framebuffer->SetTexture(CE_TEXTURE_CUBE_MAP_POSITIVE_X + i, Prefiltermap->GetRendererID(), mip);

				RenderCommand::Clear();
				cube->RenderCube();
			}
		}
		Framebuffer->Unbind();
	}

	void SkyLight::RenderSky()
	{
		backgroundShader->Bind();
		RenderCommand::BindCubemap(Cubemap->GetRendererID(), 0);
		cube->RenderCube();
		backgroundShader->Unbind();
	}
}