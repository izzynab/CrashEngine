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

		inline static void DrawInstancedTriangles(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber, unsigned int amount)
		{
			s_RendererAPI->DrawInstancedTriangles(vertexArray, trianglesNumber, amount);
		}
		
		inline static void DrawInstancedIndexed(const std::shared_ptr<VertexArray>& vertexArray, unsigned int amount, bool strip)
		{
			s_RendererAPI->DrawInstancedIndexed(vertexArray, amount, strip);
		}

		inline static void DrawInstancedLine(const std::shared_ptr<VertexArray>& vertexArray,float width, int amount)
		{
			s_RendererAPI->DrawInstancedLine(vertexArray, width, amount);
		}

		inline static void DrawLine(const std::shared_ptr<VertexArray>& vertexArray, float width)
		{
			s_RendererAPI->DrawLine(vertexArray, width);
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

		inline static void BlitFramebuffers(std::shared_ptr<Framebuffer> & readFreambuffer, std::shared_ptr<Framebuffer> & drawFramebuffer)
		{
			s_RendererAPI->BlitFramebuffers(readFreambuffer, drawFramebuffer);
		}

		inline static void Dispatch(float width, float height)
		{
			s_RendererAPI->Dispatch(width, height);
		}
		inline static void MemoryBarier()
		{
			s_RendererAPI->MemoryBarier();
		}

		inline static void GetBoolValue(uint32_t parameterValue, bool* data) { s_RendererAPI->GetBoolValue(parameterValue, data); }
		inline static void GetFloatValue(uint32_t parameterValue, float* data) { s_RendererAPI->GetFloatValue(parameterValue, data); }
		inline static void GetIntValue(uint32_t parameterValue, int* data) { s_RendererAPI->GetIntValue(parameterValue, data); }

		inline static void GetBoolIndexValue(uint32_t parameterValue, int index, bool* data) { s_RendererAPI->GetBoolIndexValue(parameterValue, index, data); }
		inline static void GetFloatIndexValue(uint32_t parameterValue, int index, float* data) { s_RendererAPI->GetFloatIndexValue(parameterValue, index, data); }
		inline static void GetIntIndexValue(uint32_t parameterValue, int index, int* data) { s_RendererAPI->GetIntIndexValue(parameterValue, index, data); }

		inline static void InitDebugOutput()
		{
			s_RendererAPI->InitDebugOutput();
		}
			
	private:
		static RendererAPI* s_RendererAPI;
	};

}