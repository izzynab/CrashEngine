#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/BasicShapes.h"
#include "CrashEngine/Renderer/Framebuffer.h"

namespace CrashEngine {

	enum class PointType
	{
		Cube,Sphere,Camera
	};

	class DebugPoint
	{
		struct Point
		{
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 color;
			float size;
			PointType type;
		};

		struct InstancedPointsDetails
		{
			std::vector<glm::mat4> matrices;
			std::vector<glm::vec4> colors;

			int Number = 0;
		};

	public:
		DebugPoint();

		void OnUpdate(Camera& camera);

		void OnFirstFrame();

		void DrawUpdatePoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size, PointType type);

		void AddPoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size, PointType type);

	private:
		std::shared_ptr<Sphere> sphere;
		std::shared_ptr<Cube> cube;

		InstancedPointsDetails cubeDetails;
		InstancedPointsDetails sphereDetails;

		Shader* instancedShader;
		Shader* shader;

		std::queue<Point> points;//todo: maybe use hash map to find has any value of point changed
	};

}

