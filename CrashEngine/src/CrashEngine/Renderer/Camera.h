#pragma once

#include "CrashEngine/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace CrashEngine {
	class CRASH_API Camera
	{
	public:

		Camera(glm::vec3 position,float Width,float Height, glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
		~Camera();

		void Update();

		void ChangeDirection(float xpos, float ypos);

		void ResetMousePos();

		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame
	
	public:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;

		float yaw = -90.0f;
		float pitch = 0.f;

		const float sensitivity = 0.1f;

		float lastX, lastY;

	private:

		bool firstMouse = true;

	};
}