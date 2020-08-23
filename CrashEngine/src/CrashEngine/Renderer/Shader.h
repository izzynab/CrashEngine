#pragma once

#include <string>

#include <glm/glm.hpp>

namespace CrashEngine {

	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformFloat(const char* name, float value);
		void SetUniformVec2(const char* name, glm::vec2 vector);
		void SetUniformVec3(const char* name, glm::vec3 vector);
		void SetUniformVec4(const char* name, glm::vec4 vector);

		void SetUniformMat3(const char* name, glm::mat4 matrix);
		void SetUniformMat4(const char* name, glm::mat4 matrix);

		uint32_t GetID();

	private:
		uint32_t m_RendererID = 0;
	};

}