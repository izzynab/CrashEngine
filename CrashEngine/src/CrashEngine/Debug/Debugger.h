#pragma once

#include "LinesDrawer.h"
#include "PointsDrawer.h"

namespace CrashEngine {
	class Debugger
	{
	public:
		static Debugger* Create();

		Debugger();

		void Begin();

		void OnUpdate(Camera& camera);

		void OnFirstFrame();

		//inline Shader& GetLinesShader() { return *debugLines->shader; }
		inline void DrawUpdateLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color = glm::vec3(0.1f, 0.1f, 0.9f), float width = 3.f) { return debugLines->DrawUpdateLine(startVec, endVec, color, width); }
		inline void DrawFrustum(Camera* camera) { debugLines->DrawFrustum(*camera); }
		inline void ClearUpdateLines() { debugLines->ClearUpdateLines(); }
		inline void AddLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color = glm::vec3(0.1f, 0.1f, 0.9f), float width = 3.f) { return debugLines->AddLine(startVec, endVec, color, width); }
		inline void AddGrid(const float gridSize = 60.f) { return debugLines->AddGrid(gridSize); }

		inline void DrawUpdatePoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size, PointType type) { debugPoints->DrawUpdatePoint(position, rotation, color, size, type); }
		inline void AddPoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size, PointType type) { debugPoints->AddPoint(position, rotation, color, size, type); }

	private:
		std::shared_ptr<DebugLine> debugLines;
		std::shared_ptr<DebugPoint> debugPoints;

	};
}

