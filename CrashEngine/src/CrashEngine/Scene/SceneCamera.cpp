#include "cepch.h"
#include "SceneCamera.h"

namespace CrashEngine {

	SceneCamera::SceneCamera()
	{

	}

	void SceneCamera::UpdateRotation(glm::vec3 cameraRotation)
	{
		Rotation = cameraRotation;

		/*glm::vec3 direction;
		direction.x = cos(glm::radians(cameraRotation.x)) * cos(glm::radians(cameraRotation.y));
		direction.y = sin(glm::radians(cameraRotation.y));
		direction.z = sin(glm::radians(cameraRotation.x)) * cos(glm::radians(cameraRotation.y));*/

		SetRotation(glm::normalize(cameraRotation));
	}

	void SceneCamera::UpdateRotation(glm::vec3 cameraRotation, glm::vec3 entityRotation)
	{
		Rotation = cameraRotation;

		/*glm::vec3 direction;
		direction.x = cos(glm::radians(cameraRotation.x)) * cos(glm::radians(cameraRotation.y));
		direction.y = sin(glm::radians(cameraRotation.y));
		direction.z = sin(glm::radians(cameraRotation.x)) * cos(glm::radians(cameraRotation.y));*/
	
		SetRotation(cameraRotation + entityRotation);
		//SetRotation(glm::normalize(direction));
	}

	void SceneCamera::UpdatePosition(glm::vec3 cameraPosition)
	{
		Position = cameraPosition;

		SetPosition(cameraPosition + entityPosition);
	}

	void SceneCamera::UpdatePosition(glm::vec3 cameraPosition, glm::vec3 entityPosition)
	{
		entityPosition = entityPosition;
		Position = cameraPosition;

		SetPosition(cameraPosition + entityPosition);
	}

}
