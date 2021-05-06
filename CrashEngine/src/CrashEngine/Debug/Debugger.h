#pragma once

#include "LinesDrawer.h"

namespace CrashEngine {
	class Debugger
	{
	public:
		static Debugger* Create();

		Debugger();

		void Begin();

		void OnUpdate(Camera& camera);

		inline Shader& GetShader() { return *debugLines->shader; }
		inline void DrawUpdateLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color = glm::vec3(0.1f, 0.1f, 0.9f), float width = 3.f) { return debugLines->DrawUpdateLine(startVec, endVec, color, width); }
		inline void ClearUpdatLines() { debugLines->ClearUpdatLines(); }
		inline void AddLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color = glm::vec3(0.1f, 0.1f, 0.9f), float width = 3.f) { return debugLines->AddLine(startVec, endVec, color, width); }
		inline void AddGrid(const float gridSize = 60.f) { return debugLines->AddGrid(gridSize); }
	private:
		std::shared_ptr<DebugLine> debugLines;
	};
}

