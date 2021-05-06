#pragma once

#include <glm/glm.hpp>

#include <string>

namespace CrashEngine {

	class Shader
	{
	public:
		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

		static Shader* Create(const char* vertexShader, const char* fragmentShader, const char* geometryShader);

		static Shader* Create(const char* vertexShader, const char* fragmentShader);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformInt(std::string name, int value) = 0;
		virtual void SetUniformFloat(std::string name, float value) = 0;
		virtual void SetUniformVec2(std::string name, glm::vec2 vector) = 0;
		virtual void SetUniformVec3(std::string name, glm::vec3 vector) = 0;
		virtual void SetUniformVec4(std::string name, glm::vec4 vector) = 0;

		virtual void SetUniformMat3(std::string name, glm::mat4 matrix) = 0;
		virtual void SetUniformMat4(std::string name, glm::mat4 matrix) = 0;

		virtual unsigned int GetID() = 0;
	};

	class ComputeShader
	{
	public:
		static ComputeShader* Create(const char* computeShader);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniformInt(std::string name, int value) = 0;

		virtual unsigned int GetID() = 0;
	};

}