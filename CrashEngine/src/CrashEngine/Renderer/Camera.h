#pragma once

#include "CrashEngine/Core/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace CrashEngine {
	class CRASH_API Camera
	{
	public:
		Camera() = default;
		Camera(glm::vec3 position, float width, float height,float nearPlane = 0.1f, float farPlane = 1500.f);
		~Camera();

		void SetProjection(glm::mat4& projection) { m_ProjectionMatrix = projection; }

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		void SetRotation(const glm::vec3& rotation) { m_Front = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		void SetSize(float height, float width);

		friend class CameraController;

	protected:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();

	public:
		float fov = 45.f;
		float ScreenWidth = 800, ScreenHeight = 800;
		float nearPlane;
		float farPlane;

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

	};
}