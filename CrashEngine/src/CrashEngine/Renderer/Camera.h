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
		Camera(glm::vec3 position, float width, float height);
		~Camera();

		void SetProjection(glm::mat4& projection) { m_ProjectionMatrix = projection; }

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		inline void SetSize(float height, float width) { ScreenHeight = height; ScreenWidth = width; RecalculateProjectionMatrix(); }
	private:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();

	public:
		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

		float fov = 45.f;
		float ScreenWidth, ScreenHeight;

	private:	
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };

	};
}