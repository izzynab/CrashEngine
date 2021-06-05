#pragma once

#include <glm/glm.hpp>
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/BasicShapes.h"

namespace CrashEngine {

	struct Line
	{
		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<VertexArray> VAO;
		std::vector<float> vertices;

		glm::vec3 color;
		float width;
	};

	struct LinesSet
	{
		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<VertexArray> VAO;
		std::vector<float> vertices;

		glm::vec3 color;
		float width;

		int NumberOfLines = 0;

		void AddLine(glm::vec3 startVec, glm::vec3 endVec)
		{
			NumberOfLines++;

			vertices.push_back(startVec.x);
			vertices.push_back(startVec.y);
			vertices.push_back(startVec.z);

			vertices.push_back(endVec.x);
			vertices.push_back(endVec.y);
			vertices.push_back(endVec.z);

			VAO.reset(VertexArray::Create());
			VBO.reset(VertexBuffer::Create(&vertices[0],vertices.size() * sizeof(float)));

			BufferLayout layout1 = {
				{ ShaderDataType::Float3, "aPosition" },
			};
			VBO->SetLayout(layout1);

			VAO->AddVertexBuffer(VBO);
		}
	};

	class DebugLine
	{
	public:
		DebugLine();

		void OnUpdate(Camera& camera);
		void OnFirstFrame();

		void DrawUpdateLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color , float width);

		void DrawFrustum(Camera& camera);

		void ClearUpdateLines();

		void AddLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color , float width);
		
		void AddGrid(const float gridSize);
	public:
		glm::mat4 view = glm::mat4(1);
		glm::mat4 projection = glm::mat4(1);
		Shader* shader;

	private:
		std::vector<Line> lines;
		std::vector<LinesSet> linesSet;

		int UpdateLinesNumber = 0;
	};

}
