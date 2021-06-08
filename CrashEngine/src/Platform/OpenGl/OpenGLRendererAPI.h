#pragma once

#include "CrashEngine/Renderer/RendererAPI.h"
#include "CrashEngine/Renderer/Framebuffer.h"

namespace CrashEngine {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual double GetTime() override;

		virtual void Enable(int nr) override;

		virtual void DepthFunc(int nr) override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, bool strip) override;
		virtual void DrawTriangles(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber) override;
		virtual void DrawLine(const std::shared_ptr<VertexArray>& vertexArray, float width) override;

		virtual void DrawInstancedTriangles(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber, unsigned int amount) override;
		virtual void DrawInstancedIndexed(const std::shared_ptr<VertexArray>& vertexArray, unsigned int amount, bool strip) override;
		virtual void DrawInstancedLine(const std::shared_ptr<VertexArray>& vertexArray, float width, int amount) override;

		virtual void BindTexture(const unsigned int& texture, unsigned int textureNr) override;
		virtual void BindCubemap(const unsigned int& texture, unsigned int textureNr) override;

		virtual void SetViewport(float Width, float Height) override;
		virtual void BlitFramebuffers(std::shared_ptr<Framebuffer>& readFreambuffer, std::shared_ptr<Framebuffer>& drawFramebuffer) override;

		virtual void Dispatch(float width, float height) override;
		virtual void MemoryBarier() override;

		virtual void GetBoolValue(uint32_t parameterValue, bool* data) override;
		virtual void GetFloatValue(uint32_t parameterValue, float* data) override;
		virtual void GetIntValue(uint32_t parameterValue, int* data) override;

		virtual void GetBoolIndexValue(uint32_t parameterValue, int index, bool* data) override;
		virtual void GetFloatIndexValue(uint32_t parameterValue, int index, float* data)override;
		virtual void GetIntIndexValue(uint32_t parameterValue, int index, int* data) override;

		virtual void InitDebugOutput() override;

	};


}