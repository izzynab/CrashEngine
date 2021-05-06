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
		glDrawArrays(GL_TRIANGLES, 0, trianglesNumber);
	}

	void OpenGLRendererAPI::DrawLine(const std::shared_ptr<VertexArray>& vertexArray,int count, float width)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glLineWidth(width);

		glDrawArrays(GL_LINES, 0, count);
		glDisable(GL_BLEND);
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

	void OpenGLRendererAPI::BlitFramebuffers(std::shared_ptr<Framebuffer>& readFreambuffer, std::shared_ptr<Framebuffer>& drawFramebuffer)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, readFreambuffer->GetRenderID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer->GetRenderID());

		glBlitFramebuffer(0, 0, readFreambuffer->GetSpecification().Width, readFreambuffer->GetSpecification().Height, 0, 0, drawFramebuffer->GetSpecification().Width, drawFramebuffer->GetSpecification().Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		//glBlitFramebuffer(0, 0, readFreambuffer->GetSpecification().Width, readFreambuffer->GetSpecification().Height, 0, 0, drawFramebuffer->GetSpecification().Width, drawFramebuffer->GetSpecification().Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	void OpenGLRendererAPI::Dispatch(float width, float height)
	{
		glDispatchCompute((GLuint)width, (GLuint)height, 1);
	}

	void OpenGLRendererAPI::MemoryBarier()
	{
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		//if (glGetError() != GL_NO_ERROR)
		{
			//CE_CORE_ERROR("Error after memery barier");
		}
	}

	void OpenGLRendererAPI::GetBoolValue(uint32_t parameterValue, bool* data)
	{
		glGetBooleanv(parameterValue, (GLboolean*)data);
	}

	void OpenGLRendererAPI::GetFloatValue(uint32_t parameterValue, float* data)
	{
		glGetFloatv(parameterValue, data);
	}

	void OpenGLRendererAPI::GetIntValue(uint32_t parameterValue, int* data)
	{
		glGetIntegerv(parameterValue, data);
	}

	void OpenGLRendererAPI::GetBoolIndexValue(uint32_t parameterValue, int index, bool* data)
	{
		glGetBooleani_v(parameterValue, index, (GLboolean*)data);
	}

	void OpenGLRendererAPI::GetFloatIndexValue(uint32_t parameterValue, int index, float* data)
	{
		glGetFloati_v(parameterValue, index, data);
	}

	void OpenGLRendererAPI::GetIntIndexValue(uint32_t parameterValue, int index, int* data)
	{
		glGetIntegeri_v(parameterValue, index, data);
	}

	void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (type == GL_DEBUG_TYPE_ERROR)
		{
			CE_CORE_ERROR("GL DEBUG ERROR: type = {0}, severity = {1}, message = {2}", type, severity, message);
		}

	}

	void OpenGLRendererAPI::InitDebugOutput()
	{
		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
	}
}