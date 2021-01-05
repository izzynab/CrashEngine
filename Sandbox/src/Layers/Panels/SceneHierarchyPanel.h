#pragma once

#include "CrashEngine/Scene/Scene.h"
#include "CrashEngine/Scene/Entity.h"
#include "MaterialPanel.h"

namespace CrashEngine {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		void SetContext(const std::shared_ptr<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		std::shared_ptr<Scene> m_Context;
		Entity m_SelectionContext;

		std::shared_ptr <MaterialPanel> MaterialPanel;
	};


}
