#pragma once

#include <glm/glm.hpp>

#include <string>

namespace CrashEngine {

	class Shader
	{
	public:
		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		static Shader* Create(const char* vertexShader, const char* fragmentShader);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformInt(const char* name, int value) = 0;
		virtual void SetUniformFloat(const char* name, float value) = 0;
		virtual void SetUniformVec2(const char* name, glm::vec2 vector) = 0;
		virtual void SetUniformVec3(const char* name, glm::vec3 vector) = 0;
		virtual void SetUniformVec4(const char* name, glm::vec4 vector) = 0;

		virtual void SetUniformMat3(const char* name, glm::mat4 matrix) = 0;
		virtual void SetUniformMat4(const char* name, glm::mat4 matrix) = 0;

		virtual unsigned int GetID() = 0;
	};

}