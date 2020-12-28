#pragma once

#include "CrashEngine/Scene/SkyLight.h"

namespace CrashEngine {

	class SceneEnvironmentPanel
	{
	public:
		SceneEnvironmentPanel() = default;
		SceneEnvironmentPanel(const std::shared_ptr<SkyLight> &skyLight);

		void OnImGuiRender();

	private:
		//std::shared_ptr<DirectionalLight> directionalLight;
		std::shared_ptr<SkyLight> m_SkyLight;
	};

}
