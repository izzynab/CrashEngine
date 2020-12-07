#include "cepch.h"
#include "Camera.h"


namespace CrashEngine {

	Camera::Camera(glm::vec3 position, float width, float height)
		: m_ProjectionMatrix(), m_ViewMatrix(1.f),Width(width), Height(height)
	{
		m_Position = position;

		RecalculateViewMatrix();
		RecalculateProjectionMatrix();
		
	}

	Camera::~Camera()
	{
	}

	void Camera::RecalculateViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}
	void Camera::RecalculateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 100.0f);
	}
}