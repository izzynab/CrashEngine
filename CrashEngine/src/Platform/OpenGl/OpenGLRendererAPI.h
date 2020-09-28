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

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
		virtual void DrawTriangles(const std::shared_ptr<VertexArray>& vertexArray) override;

		virtual unsigned int LoadTexture(std::string name) override;

		virtual void BindTexture(unsigned int& texture, unsigned int textureNr) override;
	};


}