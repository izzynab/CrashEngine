#pragma once

#include "CrashEngine/Renderer/Shader.h"

namespace CrashEngine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);

		OpenGLShader(const char* vertexShader, const char* fragmentShader);

		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniformInt(const char* name, int value) override;
		virtual void SetUniformFloat(const char* name, float value) override;
		virtual void SetUniformVec2(const char* name, glm::vec2 vector) override;
		virtual void SetUniformVec3(const char* name, glm::vec3 vector) override;
		virtual void SetUniformVec4(const char* name, glm::vec4 vector) override;
		virtual void SetUniformMat3(const char* name, glm::mat4 matrix) override;
		virtual void SetUniformMat4(const char* name, glm::mat4 matrix) override;

		virtual unsigned int GetID() override;

	private:
		unsigned int m_RendererID = 0;
	};

}