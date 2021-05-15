#pragma once

#include "CrashEngine/Core/Application.h"
#include "CrashEngine/Scene/Scene.h"

#include "Shader.h"
#include "BasicShapes.h"
#include "Framebuffer.h"


namespace CrashEngine {

	class disstream
	{
	public:
		int jd = 1;
	};

	class RenderProperties
	{
	public:
		RenderProperties();

	private:
		/*Currently active scene*/
		std::shared_ptr<Scene> m_ActiveScene;

		/*Uniform Buffer stores view and projection matrix*/
		std::shared_ptr<UniformBuffer> m_MatrixUB;

		/*GBuffer shader gives buffers to deffered render*/
		std::shared_ptr<Shader> GBufferShader;

		/*Shader draws pbr scene from gbuffer images*/
		std::shared_ptr<Shader> deferredShader;

		/*Shader draws pbr scene from textures of models*/
		std::shared_ptr<Shader> forwardShader;

		/*Framebuffer which stores GBuffer images*/
		std::shared_ptr<Framebuffer> deferredframebuffer;

		std::shared_ptr<Quad> quad;

	public:
		inline std::shared_ptr<Scene> GetScene() { return m_ActiveScene; }
		inline std::shared_ptr<UniformBuffer> GetUBO() { return m_MatrixUB; }
		inline std::shared_ptr<Framebuffer> GetDefferedFramebuffer() { return deferredframebuffer; }

		inline std::shared_ptr<Shader> GetDefferedShader() { return deferredShader; }
		inline std::shared_ptr<Shader> GetGBufferShader() { return GBufferShader; }

		inline std::shared_ptr<Quad> GetQuad() { return quad; }
	};
}