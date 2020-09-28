#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

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

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;
		virtual void DrawTriangles(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		virtual unsigned int LoadTexture(std::string name) = 0;
		virtual void BindTexture(unsigned int& texture, unsigned int textureNr) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}