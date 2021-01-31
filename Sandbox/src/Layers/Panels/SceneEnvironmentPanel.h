#pragma once

#include "CrashEngine/Scene/SkyLight.h"
#include "CrashEngine/Scene/DirectionalLight.h"

namespace CrashEngine {

	class SceneEnvironmentPanel
	{
	public:
		SceneEnvironmentPanel() = default;
		SceneEnvironmentPanel(const std::shared_ptr<SkyLight> &skyLight, const std::shared_ptr<DirectionalLight>& directionalLight, const std::shared_ptr<Scene> &m_ActiveScene);

		void OnImGuiRender();

	private:
		std::shared_ptr<DirectionalLight> m_DirectionalLight;
		std::shared_ptr<SkyLight> m_SkyLight;
		std::shared_ptr<Scene> m_Scene;
	};

}
