#pragma once

#include "CrashEngine/Renderer/Camera.h"

namespace CrashEngine {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void UpdateRotation(glm::vec3 rotation);
		void UpdatePosition(glm::vec3 position);

		glm::vec3 Rotation = glm::vec3(1.f, 0.f, 0.f);
		glm::vec3 Position = glm::vec3(0.f, 0.f, 0.f);
	};

}
