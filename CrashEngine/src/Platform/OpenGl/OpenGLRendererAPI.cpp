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


	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawTriangles(const std::shared_ptr<VertexArray>& vertexArray)
	{
		//glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetIndexBuffer()->GetCount());
		glDrawElements(GL_TRIANGLE_STRIP, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	unsigned int OpenGLRendererAPI::LoadTexture(std::string name)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);

		std::string path = "C:\\EngineDev\\CrashEngine\\Textures\\" + name;

		int width, height, nrComponents;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			CE_CORE_ERROR("Texture failed to load at path: {}", path);
			stbi_image_free(data);
		}

		return textureID;
	}

	void OpenGLRendererAPI::BindTexture(unsigned int& texture,unsigned int textureNr)
	{
		glActiveTexture(0x84C0 + textureNr);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
}