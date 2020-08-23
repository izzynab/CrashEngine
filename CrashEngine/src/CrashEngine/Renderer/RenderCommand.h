#pragma once

#include "RendererAPI.h"

namespace CrashEngine {

	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawTriangles(const std::shared_ptr<VertexArray>& vertexArray, unsigned int NumberOfVerticies)
		{
			s_RendererAPI->DrawTriangles(vertexArray, NumberOfVerticies);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};

}