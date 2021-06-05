#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/BasicShapes.h"

namespace CrashEngine {

	enum class PointType
	{
		Cube,Sphere,Camera
	};

	/*struct Point
	{
		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 color;
		float size;
		PointType type;
	};*/

	class DebugPoint
	{
	public:
		DebugPoint();



		void OnUpdate(Camera& camera);

		void DrawUpdatePoint(glm::vec3 pos, glm::vec3 rot, glm::vec3 color, float size, PointType type);

		void ClearUpdatePoints();

		void AddPoint(glm::vec3 pos, glm::vec3 rot, glm::vec3 color, float size, PointType type);

	public:
		glm::mat4 view = glm::mat4(1);
		glm::mat4 projection = glm::mat4(1);
		Shader* shader;

	private:
		//int UpdatePointsNumber = 0;
		int CubesNumber = 0;

		//std::shared_ptr<Sphere> sphere;

		std::shared_ptr<Cube> cube;

		std::vector<glm::mat4> matrices;
		std::vector<glm::vec4> colors;

		//std::vector<Point> points;
	};

}

