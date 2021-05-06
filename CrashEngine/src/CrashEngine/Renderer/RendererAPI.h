#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "Framebuffer.h"

namespace CrashEngine {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual double GetTime() = 0;

		virtual void Enable(int nr) = 0;

		virtual void DepthFunc(int nr) = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, bool strip) = 0;
		virtual void DrawTriangles(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber) = 0;
		virtual void DrawLine(const std::shared_ptr<VertexArray>& vertexArray, int count, float width) = 0;

		virtual void BindTexture(const unsigned int& texture, unsigned int textureNr) = 0;
		virtual void BindCubemap(const unsigned int& texture, unsigned int textureNr) = 0;

		virtual void SetViewport(float Width, float Height) = 0;
		virtual void BlitFramebuffers(std::shared_ptr<Framebuffer>& readFreambuffer, std::shared_ptr<Framebuffer>& drawFramebuffer) = 0;

		virtual void Dispatch(float width,float height) = 0;
		virtual void MemoryBarier() = 0;

		virtual void GetBoolValue(uint32_t parameterValue, bool* data) = 0;
		virtual void GetFloatValue(uint32_t parameterValue, float* data) = 0;
		virtual void GetIntValue(uint32_t parameterValue, int* data) = 0;

		virtual void GetBoolIndexValue(uint32_t parameterValue,int index, bool* data) = 0;
		virtual void GetFloatIndexValue(uint32_t parameterValue, int index, float* data) = 0;
		virtual void GetIntIndexValue(uint32_t parameterValue, int index, int* data) = 0;

		virtual void InitDebugOutput() = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};

}