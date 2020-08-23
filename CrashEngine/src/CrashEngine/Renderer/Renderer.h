#pragma once

#include "RenderCommand.h"

namespace CrashEngine {

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void SubmitDebug(const std::shared_ptr<VertexArray>& vertexArray, unsigned int NumberOfVerticies);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};


}