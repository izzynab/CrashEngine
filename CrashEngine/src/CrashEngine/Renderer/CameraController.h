#pragma once

#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Core/Timestep.h"

#include "CrashEngine/Events/ApplicationEvent.h"
#include "CrashEngine/Events/MouseEvent.h"

namespace CrashEngine {
	class CameraController
	{
	public:
		CameraController() = default;
		CameraController(std::shared_ptr<Camera>& camera);

		void SetCamera(std::shared_ptr<Camera>& camera);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		float m_CameraSpeed = 30.0f;
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Camera* m_Camera;

		bool firstMouse = true;
		float sensitivity = 0.1f;

		float yaw = -90.0f;
		float pitch = 0.f;

		float lastX, lastY;

	};
}
