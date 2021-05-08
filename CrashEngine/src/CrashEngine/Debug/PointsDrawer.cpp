#include "cepch.h"
#include "PointsDrawer.h"

namespace CrashEngine {
	DebugPoint::DebugPoint()
	{
		shader = Shader::Create("basic3d.vert", "basic3d.frag");
		cube.reset(new Cube());
		sphere.reset(new Sphere());
	}

	void DebugPoint::OnUpdate(Camera& camera)
	{
		for (int i = 0; i < points.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(points[i].size));
			model = glm::translate(model, points[i].pos);
			shader->Bind();
			shader->SetUniformVec3("color", points[i].color);
			shader->SetUniformMat4("model", model);

			switch (points[i].type)
			{
			case PointType::Cube:
				cube->RenderCube();
				break;
			case PointType::Sphere:
				sphere->RenderSphere();
				break;
			}
		}
	}

	void DebugPoint::DrawUpdatePoint(glm::vec3 pos, glm::vec3 color, float size, PointType type)
	{
		UpdatePointsNumber++;

		AddPoint(pos, color, size, type);
	}

	void DebugPoint::ClearUpdatePoints()
	{
		points.erase(points.end() - UpdatePointsNumber, points.end());
		UpdatePointsNumber = 0;
	}

	void DebugPoint::AddPoint(glm::vec3 pos, glm::vec3 color, float size, PointType type)
	{
		Point point;
		point.color = color;
		point.pos = pos;
		point.size = size;
		point.type = type;

		points.push_back(point);
	}
}