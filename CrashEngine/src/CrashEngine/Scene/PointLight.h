#pragma once
#include "glm/glm.hpp"

namespace CrashEngine {

	class PointLight
	{
	public:
		PointLight();

		float intensity = 10.f;
		glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	};
}