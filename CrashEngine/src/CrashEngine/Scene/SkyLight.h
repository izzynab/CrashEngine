#pragma once

#include "CrashEngine/Core/Core.h"

#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"


namespace CrashEngine {

	class SkyLight
	{
	public:
		SkyLight();

		void LoadHDR(std::string path);

		void UpdateCubemap();

		void RenderSky();

		inline void BindIrradianceMap(uint32_t textureNr){ RenderCommand::BindCubemap(Irradiancemap->GetRendererID(), textureNr); }
		inline void BindPrefilterMap(uint32_t textureNr) { RenderCommand::BindCubemap(Prefiltermap->GetRendererID(), textureNr); }
		inline void BindbrdfTexture(uint32_t textureNr) { RenderCommand::BindTexture(brdfLUTTexture->GetRendererID(), textureNr); }

		inline Shader* GetSkyShader() { return backgroundShader; }

	public:
		std::shared_ptr<Texture2D> brdfLUTTexture;

		std::string directory;

		int BlurSamples = 0;

	private:
		std::shared_ptr<TextureHDR> HDR;
		std::shared_ptr<CubemapTexture> Cubemap;
		std::shared_ptr<CubemapTexture> Irradiancemap;
		std::shared_ptr<CubemapTexture> Prefiltermap;//todo: should be better resolution

		Shader* equirectangularToCubemapShader;
		Shader* irradianceShader;
		Shader* prefilterShader;
		Shader* brdfShader;
		Shader* backgroundShader;

		std::shared_ptr<Framebuffer> Framebuffer;
		std::shared_ptr<Renderbuffer> Renderbuffer;

		std::shared_ptr<Cube> cube;
		std::shared_ptr<Quad> quad;

		std::shared_ptr<TextureCreator> texCreator;

	private:
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[6] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

	};
}