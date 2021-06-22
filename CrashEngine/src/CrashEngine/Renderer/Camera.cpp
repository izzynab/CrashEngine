#include "cepch.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace CrashEngine {

	Camera::Camera(glm::vec3 position, float width, float height, float nearPlane, float farPlane)
		: m_ProjectionMatrix(), m_ViewMatrix(1.f), ScreenWidth(width), ScreenHeight(height), m_Position(position), nearPlane(nearPlane), farPlane(farPlane)
	{
		RecalculateViewMatrix();
		RecalculateProjectionMatrix();
	}

	Camera::~Camera()
	{
	}

	void Camera::SetSize(float height, float width)
	{
		if(height == ScreenHeight && width == ScreenWidth) return;
		ScreenHeight = height;
		ScreenWidth = width; 
		RecalculateProjectionMatrix();
	}

	void Camera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}
	void Camera::RecalculateProjectionMatrix()
	{
		if (ScreenWidth <= 0 || ScreenHeight <= 0) return;
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), ScreenWidth / ScreenHeight, nearPlane, farPlane);
	}


}