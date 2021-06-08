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
		static void RenderScene(std::shared_ptr<RenderProperties>& renderProperties, std::vector<std::shared_ptr<Framebuffer>> &renderFramebuffers, Timestep ts);
		static void EndScene();

		static void AddView(float width,float height, std::string name,std::shared_ptr<RenderProperties>& renderProperties, std::vector<std::shared_ptr<Framebuffer>>& renderFramebuffers);

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, bool strip = false);
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber);
		//static void SubmitDebug(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber);
		static void SubmitLine(const std::shared_ptr<VertexArray>& vertexArray, int count, float width);

		static void SubmitInstanced(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber, unsigned int amount);
		static void SubmitInstancedIndexed(const std::shared_ptr<VertexArray>& vertexArray, unsigned int amount, bool strip = false);
		static void SubmitInstancedLine(const std::shared_ptr<VertexArray>& vertexArray,  float width, int amount);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};


}