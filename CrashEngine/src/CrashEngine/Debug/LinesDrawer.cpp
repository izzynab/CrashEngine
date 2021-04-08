#include "cepch.h"
#include "LinesDrawer.h"

#include "CrashEngine/Renderer/Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace CrashEngine {
	DebugLine::DebugLine()
	{
		shader = Shader::Create("lines.vert", "lines.frag");
	}

	void DebugLine::DrawDebugLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color, float width)
	{
		CE_CORE_TRACE("Debug_line_start: x:{0} y:{1} z:{2}", startVec.x, startVec.y, startVec.z);
		CE_CORE_TRACE("Debug_line_end: x:{0} y:{1} z:{2}", endVec.x, endVec.y, endVec.z);

		vertices = {
			 startVec.x, startVec.y, startVec.z,
			 endVec.x, endVec.y, endVec.z,
		};

		VAO.reset(VertexArray::Create());
		VBO.reset(VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float)));

		BufferLayout layout1 = {
			{ ShaderDataType::Float3, "aPosition" },
		};
		VBO->SetLayout(layout1);

		VAO->AddVertexBuffer(VBO);

		glm::mat4 model = glm::mat4(1.0f);
		shader->Bind();
		shader->SetUniformVec3("color", color);
		shader->SetUniformMat4("model", model);

		Renderer::SubmitLine(VAO, width);

	}
}