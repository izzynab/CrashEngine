#pragma once

#include <glm/glm.hpp>
#include "CrashEngine/Renderer/Shader.h"

#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"

namespace CrashEngine {

	class DebugLine
	{
	public:
		DebugLine();

		void DrawDebugLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color = glm::vec3(0.1f,0.1f,0.9f), float width = 3.f);

	public:
		glm::mat4 view = glm::mat4(1);
		glm::mat4 projection = glm::mat4(1);
		Shader* shader;

		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<VertexArray> VAO;
		std::vector<float> vertices;
	};
}
