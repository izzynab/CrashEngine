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
		struct view
		{
			uint32_t id;

			/*Framebuffer for capture ssao texture*/
			std::shared_ptr<Framebuffer> ssaoFramebuffer;

			/*Framebuffer for blurring ssao texture*/
			std::shared_ptr<Framebuffer> ssaoBlurFramebuffer;
		};

	public:
		SSAO();

		void Render(float width, float height, uint32_t GBufferPosition, uint32_t GBufferNormals,uint32_t viewID = 0);

		void AddView(float width, float height, uint32_t id);

		inline uint32_t GetShaderID() { return ssaoShader->GetID(); }
		inline uint32_t GetTextureRendererID(uint32_t viewID) { return views[viewID].ssaoBlurFramebuffer->GetColorAttachmentRendererID(); }

	public:
		int kernelSize = 64;
		float radius = 0.5;
		float bias = 0.025;
		float power = 3.f;

		bool ssao = true;

	private:
		glm::vec2 noiseScale = glm::vec2(1920.0 / 4.0, 1080.0 / 4.0);
		std::shared_ptr<Texture2D> noiseTexture;
		std::vector<glm::vec3> ssaoKernel;

		/*ssao texture shader*/
		Shader* ssaoShader;

		/*Shader blures ssao texture*/
		Shader* ssaoBlurShader;

		std::vector<view> views;
		std::shared_ptr<Quad> quad;

	};
}
