#include "cepch.h"
#include "Camera.h"

CrashEngine::Camera::Camera(glm::vec3 position, float Width, float Height, glm::vec3 front, glm::vec3 up)
{
	Position = position;
	Front = front;
	Up = up;

	lastX = Width / 2;
	lastY = Height / 2;
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
