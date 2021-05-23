#include "cepch.h"
#include "SceneCamera.h"

namespace CrashEngine {

	SceneCamera::SceneCamera()
	{

	}

	void SceneCamera::UpdateRotation(glm::vec3 rotation)
	{
		Rotation = rotation;

		glm::vec3 direction;
		direction.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
		direction.y = sin(glm::radians(rotation.y));
		direction.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	
		SetRotation(glm::normalize(direction));
	}

	void SceneCamera::UpdatePosition(glm::vec3 position)
	{
		Position = position;

		SetPosition(position);
	}

}
