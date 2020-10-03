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

		inline static double GetTime()
		{
			return s_RendererAPI->GetTime();
		}

		inline static void Enable(int nr)
		{
			s_RendererAPI->Enable(nr);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawTriangles(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawTriangles(vertexArray);
		}

		inline static void BindTexture(const unsigned int& texture,unsigned int textureNr)
		{
			return s_RendererAPI->BindTexture(texture, textureNr);
		}

		inline static void SetViewport(float Width,float Height)
		{
			s_RendererAPI->SetViewport(Width, Height);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}