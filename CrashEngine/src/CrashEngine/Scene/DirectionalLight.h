#pragma once

#include <glm/gtc/type_ptr.hpp>

namespace CrashEngine {

	class DirectionalLight
	{
	public:
		glm::vec3 rotation = glm::vec3(0,0.02,0);
		glm::vec3 color = glm::vec3(1,1,1);
		float intensity = 50.f;
	};
}