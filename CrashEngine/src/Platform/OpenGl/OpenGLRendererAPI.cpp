#include "cepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "stb_image.h"

namespace CrashEngine {

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	double OpenGLRendererAPI::GetTime()
	{
		return glfwGetTime();
	}

	void OpenGLRendererAPI::Enable(int nr)
	{
		glEnable(nr);
	}

	void OpenGLRendererAPI::DepthFunc(int nr)
	{
		glDepthFunc(nr);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, bool strip)
	{
		if (strip)
		{
			glDrawElements(GL_TRIANGLE_STRIP, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void OpenGLRendererAPI::DrawTriangles(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber)
	{
		//TODO: make this works not just for cube
		glDrawArrays(GL_TRIANGLES, 0, trianglesNumber);
	}

	void OpenGLRendererAPI::BindTexture(const unsigned int& texture, unsigned int textureNr)
	{
		glActiveTexture(0x84C0 + textureNr);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void OpenGLRendererAPI::BindCubemap(const unsigned int& texture, unsigned int textureNr)
	{
		glActiveTexture(0x84C0 + textureNr);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	}

	void OpenGLRendererAPI::SetViewport(float Width, float Height)
	{
		glViewport(0, 0, Width, Height);
	}
}