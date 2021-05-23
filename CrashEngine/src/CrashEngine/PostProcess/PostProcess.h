#pragma once

#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Framebuffer.h"
#include "CrashEngine/Renderer/BasicShapes.h"

namespace CrashEngine {

	class PostProcess
	{
		struct view
		{
			uint32_t id;

			std::shared_ptr<Framebuffer> blurFramebuffer[2];
			std::shared_ptr<Framebuffer> bloom;
			std::shared_ptr<Framebuffer> draw_framebuffer;
		};

	public:
		PostProcess();

	public:
		/*FXAA  must be before any other post process*/
		void ApplyFXAA(std::shared_ptr<Framebuffer>& framebuffer, uint32_t viewID = 0);

		/*Function blurs image*/
		void Blur(std::shared_ptr<Framebuffer> &framebuffer, uint32_t viewID = 0);

		/*Gamma Correction and tone mapping on image*/
		void GammaHDRCorretion(std::shared_ptr<Framebuffer>& framebuffer, uint32_t viewID = 0);

		void AddView(float width, float height,  uint32_t id);

	public:
		bool blur = true;
		float blurThreshold = 0.4f; 
		float blurSoftThreshold = 0.1f;

		float exposure = 1.f;

	private:
		Shader* blurShader;
		Shader* BloomMixShader;
		Shader* brightShader;
		Shader* HDRGammaCorrection;
		Shader* FXAAShader;

		std::vector<view> views;
		std::shared_ptr<Quad> quad;
	};
}