#pragma once

#include "LinesDrawer.h"
#include "PointsDrawer.h"
#include "MeshDrawer.h"

namespace CrashEngine {
	class Debugger
	{
	public:
		static Debugger* Create();

		Debugger();

		void Begin();

		void OnUpdate(Camera& camera);

		inline Shader& GetLinesShader() { return *debugLines->shader; }
		inline void DrawUpdateLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color = glm::vec3(0.1f, 0.1f, 0.9f), float width = 3.f) { return debugLines->DrawUpdateLine(startVec, endVec, color, width); }
		inline void DrawFrustum(Camera* camera) { debugLines->DrawFrustum(*camera); }
		inline void ClearUpdateLines() { debugLines->ClearUpdateLines(); }
		inline void AddLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color = glm::vec3(0.1f, 0.1f, 0.9f), float width = 3.f) { return debugLines->AddLine(startVec, endVec, color, width); }
		inline void AddGrid(const float gridSize = 60.f) { return debugLines->AddGrid(gridSize); }

		inline Shader& GetPointsShader() { return *debugPoints->shader; }
		inline void DrawUpdatePoint(glm::vec3 pos, glm::vec3 color, float size, PointType type) { debugPoints->DrawUpdatePoint(pos, color, size, type); }
		inline void ClearUpdatePoints() { debugPoints->ClearUpdatePoints(); }
		inline void AddPoint(glm::vec3 pos, glm::vec3 color, float size, PointType type) { debugPoints->AddPoint(pos, color, size, type); }

		//inline Shader& GetPointsShader() {; }
		inline void DrawUpdateMesh() { ; }
		inline void ClearUpdateMeshes() { ; }
		inline void AddMesh() { ; }
	private:
		std::shared_ptr<DebugLine> debugLines;
		std::shared_ptr<DebugPoint> debugPoints;
		std::shared_ptr<DebugMesh> debugMeshes;
	};
}

