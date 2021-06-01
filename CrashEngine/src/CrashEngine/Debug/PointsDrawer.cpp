#include "cepch.h"
#include "PointsDrawer.h"

#include "CrashEngine/Renderer/Buffer.h"

namespace CrashEngine {
	DebugPoint::DebugPoint()
	{
		shader = Shader::Create("Instanced3D.vert", "Instanced3D.frag");
		cube.reset(new Cube());
		//sphere.reset(new Sphere());
	}

	void DebugPoint::OnUpdate(Camera& camera)
	{			
		shader->Bind();
		cube->RenderInstancedCube(CubesNumber);
		
	}

	void DebugPoint::DrawUpdatePoint(glm::vec3 pos, glm::vec3 rot, glm::vec3 color, float size, PointType type)
	{
		/*UpdatePointsNumber++;

		AddPoint(pos,rot, color, size, type);*/
	}

	void DebugPoint::ClearUpdatePoints()
	{
		/*points.erase(points.end() - UpdatePointsNumber, points.end());
		UpdatePointsNumber = 0;*/
	}

	void DebugPoint::AddPoint(glm::vec3 pos, glm::vec3 rot, glm::vec3 color, float size, PointType type)
	{
		CubesNumber++;

		colors.push_back(glm::vec4(color,1));

		glm::mat4 rotation = glm::toMat4(glm::quat(rot));

		glm::mat4 model =  glm::translate(glm::mat4(1.0f), pos)
			* rotation
			* glm::scale(glm::mat4(1.0f), glm::vec3(size));

		matrices.push_back(model);

		/*Point point;
		point.color = color;
		point.pos = pos;
		point.size = size;
		point.type = type;

		points.push_back(point);*/

		std::shared_ptr<VertexBuffer> ColorVB;
		ColorVB.reset(VertexBuffer::Create(glm::value_ptr(colors[0]), colors.size() * sizeof(glm::vec4)));

		BufferLayout layout2 = {
			{ ShaderDataType::Float4, "aColor"},
		};
		ColorVB->SetLayout(layout2);

		cube->GetVertexArray()->AddInstancedVertexBuffer(ColorVB,3);

		std::shared_ptr<VertexBuffer> MatrixVB;
		MatrixVB.reset(VertexBuffer::Create(glm::value_ptr(matrices[0]), matrices.size() * sizeof(glm::mat4)));

		BufferLayout layout1 = {
			{ ShaderDataType::Mat4, "aInstanceMatrix" },
		};
		MatrixVB->SetLayout(layout1);

		cube->GetVertexArray()->AddInstancedVertexBuffer(MatrixVB,4);
	}
}