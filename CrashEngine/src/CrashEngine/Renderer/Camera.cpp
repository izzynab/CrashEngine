#include "cepch.h"
#include "Camera.h"

CrashEngine::Camera::Camera(glm::vec3 position, glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
{
	Position = position;
	Front = front;
	Up = up;
}

CrashEngine::Camera::~Camera()
{
}

void CrashEngine::Camera::ChangeDirection(float xpos, float ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;

		firstMouse = false;
	}


	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front = glm::normalize(direction);
}

void CrashEngine::Camera::ResetMousePos()
{
	firstMouse = true;
}
