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

		inline static unsigned int LoadTexture(std::string name)
		{
			return s_RendererAPI->LoadTexture(name);
		}

		inline static void BindTexture(unsigned int& texture, unsigned int textureNr)
		{
			s_RendererAPI->BindTexture(texture, textureNr);
		}


	private:
		static RendererAPI* s_RendererAPI;
	};

}