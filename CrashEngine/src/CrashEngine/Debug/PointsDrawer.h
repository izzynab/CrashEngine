#pragma once

#include <glm/glm.hpp>
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/BasicShapes.h"

namespace CrashEngine {

	class DebugPoint
	{
	public:
		DebugPoint();

		void OnUpdate(Camera& camera);

		void DrawUpdateLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color, float width);

		void ClearUpdatPoint();

		void AddPoint(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color, float width);

	public:
		glm::mat4 view = glm::mat4(1);
		glm::mat4 projection = glm::mat4(1);
		Shader* shader;

	private:
		//std::shared_ptr<Line> UpdateLine;

		std::vector<Line> lines;

		std::vector<LinesSet> linesSet;

		int UpdateLinesNumber = 0;
	};

}

