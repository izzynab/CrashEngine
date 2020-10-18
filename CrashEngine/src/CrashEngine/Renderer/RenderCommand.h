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

		inline static void DepthFunc(int nr)
		{
			s_RendererAPI->DepthFunc(nr);
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, bool strip)
		{
			s_RendererAPI->DrawIndexed(vertexArray, strip);
		}

		inline static void DrawTriangles(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber)
		{
			s_RendererAPI->DrawTriangles(vertexArray, trianglesNumber);
		}

		inline static void BindTexture(const uint32_t& texture,uint32_t textureNr)
		{
			return s_RendererAPI->BindTexture(texture, textureNr);
		}

		inline static void BindCubemap(const uint32_t& texture, uint32_t textureNr)
		{
			return s_RendererAPI->BindCubemap(texture, textureNr);
		}

		inline static void SetViewport(float Width,float Height)
		{
			s_RendererAPI->SetViewport(Width, Height);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}