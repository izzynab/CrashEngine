#pragma once

#include "Scene.h"
#include "DirectionalLight.h"
#include "SkyLight.h"
#include "Mesh.h"

namespace CrashEngine {

	class SceneSerializer
	{
	public:
		SceneSerializer(const std::shared_ptr<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		std::shared_ptr<Scene> m_Scene;
		std::shared_ptr<DirectionalLight> m_directionalLight;
		std::shared_ptr<SkyLight> m_skyLight;
	};

}
