#pragma once

#include "CrashEngine/Renderer/Shader.h"

namespace CrashEngine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);

		OpenGLShader(const char* vertexShader, const char* fragmentShader, const char* geometryShader);

		OpenGLShader(const char* vertexShader, const char* fragmentShader);

		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniformInt	(std::string name, int value) override;
		virtual void SetUniformFloat(std::string name, float value) override;
		virtual void SetUniformVec2(std::string name, glm::vec2 vector) override;
		virtual void SetUniformVec3(std::string name, glm::vec3 vector) override;
		virtual void SetUniformVec4(std::string name, glm::vec4 vector) override;
		virtual void SetUniformMat3(std::string name, glm::mat4 matrix) override;
		virtual void SetUniformMat4(std::string name, glm::mat4 matrix) override;

		virtual unsigned int GetID() override;

	private:
		unsigned int m_RendererID = 0;
	};

}