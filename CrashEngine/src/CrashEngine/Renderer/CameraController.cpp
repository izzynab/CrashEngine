#include "cepch.h"
#include "CameraController.h"

#include "CrashEngine/Core/Input.h"
#include "CrashEngine/Core/KeyCodes.h"
#include "CrashEngine/Core/MouseButtonCodes.h"

namespace CrashEngine {

	CameraController::CameraController(glm::vec3 position, float width, float height)
		: m_Camera(position, width, height)
	{
		lastX = width / 2;
		lastY = height / 2;


	}



	void CameraController::OnUpdate(Timestep ts)
	{
		float cameraSpeed = m_CameraSpeed * ts.GetSeconds();
		if (Input::IsMouseButtonPressed(CE_MOUSE_BUTTON_RIGHT))
		{
			if (Input::IsKeyPressed(CE_KEY_W))
			{
				m_CameraPosition += cameraSpeed * m_Camera.m_Front;
			}
			if (Input::IsKeyPressed(CE_KEY_S))
			{
				m_CameraPosition -= cameraSpeed * m_Camera.m_Front;
			}
			if (Input::IsKeyPressed(CE_KEY_A))
			{
				m_CameraPosition -= glm::normalize(glm::cross(m_Camera.m_Front, m_Camera.m_Up)) * cameraSpeed;
			}
			if (Input::IsKeyPressed(CE_KEY_D))
			{
				m_CameraPosition += glm::normalize(glm::cross(m_Camera.m_Front, m_Camera.m_Up)) * cameraSpeed;
			}
			
			float xpos = Input::GetMouseX();
			float ypos = Input::GetMouseY();

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
			m_Camera.m_Front = glm::normalize(direction);
		}
		else
		{
			firstMouse = true;
		}
	

		m_Camera.SetPosition(m_CameraPosition);
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(CE_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(CE_BIND_EVENT_FN(CameraController::OnWindowResized));
	}

	void CameraController::OnResize(float width, float height)
	{
		m_Camera.SetSize(height, width);
		lastX = width / 2;
		lastY = height / 2;
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		//todo: not sure if this works
		m_CameraSpeed -= e.GetYOffset() * 0.25f;
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}