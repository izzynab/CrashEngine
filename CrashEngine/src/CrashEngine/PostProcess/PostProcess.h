#pragma once

#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Framebuffer.h"
#include "CrashEngine/Renderer/BasicShapes.h"

namespace CrashEngine {

	class PostProcess
	{
	public:
		PostProcess();

	public:
		/*FXAA  must be before any other post process*/
		/*May be good looking post process when approximate 4 near pixels colors to one pixel*/
		/*Some kind of sobe filter???*/
		void ApplyFXAA(std::shared_ptr<Framebuffer>& framebuffer);

		/*Function blurs image*/
		void Blur(std::shared_ptr<Framebuffer> &framebuffer);

		/*Gamma Correction and tone mapping on image*/
		void GammaHDRCorretion(std::shared_ptr<Framebuffer>& framebuffer);



	public:
		bool blur = true;
		float exposure = 1.f; 

	private:
		Shader* blurShader;
		Shader* BloomMixShader;
		Shader* brightShader;
		Shader* HDRGammaCorrection;
		Shader* FXAAShader;

		std::shared_ptr<Framebuffer> blurFramebuffer[2];
		std::shared_ptr<Framebuffer> bloom;
		std::shared_ptr<Framebuffer> draw_framebuffer;
		std::shared_ptr<Quad> quad;
	};
}