#pragma once

#include "CrashEngine/Renderer/RendererAPI.h"

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

		virtual void BindTexture(const unsigned int& texture, unsigned int textureNr) override;
		virtual void BindCubemap(const unsigned int& texture, unsigned int textureNr) override;

		virtual void SetViewport(float Width, float Height) override;
	};


}