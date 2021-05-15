#pragma once

#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Core/Timestep.h"

#include "CrashEngine/Events/ApplicationEvent.h"
#include "CrashEngine/Events/MouseEvent.h"

namespace CrashEngine {
	class CameraController
	{
	public:
		CameraController(glm::vec3 position, float width, float height);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		float m_CameraSpeed = 30.0f;
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Camera m_Camera;

		glm::vec3 m_CameraPosition = { -10.0f, 20.0f, 0.0f };
		bool firstMouse = true;
		float sensitivity = 0.1f;

		float yaw = -90.0f;
		float pitch = 0.f;

		float lastX, lastY;

	};
}
