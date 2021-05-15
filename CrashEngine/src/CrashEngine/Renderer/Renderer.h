#pragma once

#include "RenderCommand.h"
#include "CameraController.h"
//#include "RenderProperties.h"

namespace CrashEngine {
	class RenderProperties;

	class Renderer
	{
	public:
		static void BeginScene();
		//static void RenderScene(std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<Framebuffer> deferredframebuffer,float Width,float Height,Camera *camera, std::shared_ptr<UniformBuffer> uniformBuffer, std::shared_ptr<Scene> m_ActiveScene,Timestep ts);
		static void RenderScene(std::shared_ptr<RenderProperties>& renderProperties, std::shared_ptr<Framebuffer>& renderFramebuffer, Camera* camera, Timestep ts);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, bool strip = false);
		static void SubmitDebug(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber);
		static void SubmitLine(const std::shared_ptr<VertexArray>& vertexArray, int count, float width);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};


}