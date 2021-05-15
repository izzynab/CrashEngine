#pragma once

#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Renderer/Texture.h"
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/BasicShapes.h"
#include "CrashEngine/Renderer/Framebuffer.h"

namespace CrashEngine
{
	class SSAO
	{
	public:
		SSAO();

		void Render(float Width,float Height, uint32_t GBufferPosition, uint32_t GBufferNormals);

		inline uint32_t GetTextureRendererID() { return ssaoBlurFramebuffer->GetColorAttachmentRendererID(); }

	public:
		/*Shader draws ssao texture*/
		Shader* ssaoShader;

		int kernelSize = 64;
		float radius = 0.5;
		float bias = 0.025;
		float power = 3.f;

		bool ssao = true;

	private:
		glm::vec2 noiseScale = glm::vec2(1920.0 / 4.0, 1080.0 / 4.0);

	private:
		/*Framebuffer for capture ssao texture*/
		std::shared_ptr<Framebuffer> ssaoFramebuffer;

		/*Framebuffer for capture ssao texture*/
		std::shared_ptr<Framebuffer> ssaoBlurFramebuffer;

		std::vector<glm::vec3> ssaoKernel;

		std::shared_ptr<Texture2D> noiseTexture;

		/*Shader blures ssao texture*/
		Shader* ssaoBlurShader;

		std::shared_ptr<Quad> quad;

	};
}
