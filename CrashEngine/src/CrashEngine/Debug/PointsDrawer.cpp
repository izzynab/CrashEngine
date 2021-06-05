#include "cepch.h"
#include "PointsDrawer.h"

#include "CrashEngine/Renderer/Buffer.h"

namespace CrashEngine {
	DebugPoint::DebugPoint()
	{
		instancedShader = Shader::Create("Instanced3D.vert", "Instanced3D.frag");
		shader = Shader::Create("Basic3D.vert", "Basic3D.frag");
		cube.reset(new Cube());
		//sphere.reset(new Sphere());
	}

	void DebugPoint::OnUpdate(Camera& camera)
	{			
		instancedShader->Bind();
		cube->RenderInstancedCube(CubesNumber);

		int size = points.size();
		for (int i = 0; i < size; i++)
		{
			shader->Bind();
			shader->SetUniformVec3("color", points.front().color);
			glm::mat4 rotation = glm::toMat4(glm::quat(points.front().rotation));
			glm::mat4 model = glm::translate(glm::mat4(1.0f), points.front().position)
				* rotation
				* glm::scale(glm::mat4(1.0f), glm::vec3(points.front().size));
			shader->SetUniformMat4("model", model);
			cube->RenderCube();

			points.pop();
		}
	}

	void DebugPoint::OnFirstFrame()
	{
		if (colors.empty() || matrices.empty())return;
		std::shared_ptr<VertexBuffer> ColorVB;
		ColorVB.reset(VertexBuffer::Create(glm::value_ptr(colors[0]), colors.size() * sizeof(glm::vec4)));

		BufferLayout layout2 = {
			{ ShaderDataType::Float4, "aColor"},
		};
		ColorVB->SetLayout(layout2);

		cube->GetVertexArray()->AddInstancedVertexBuffer(ColorVB, 3);

		std::shared_ptr<VertexBuffer> MatrixVB;
		MatrixVB.reset(VertexBuffer::Create(glm::value_ptr(matrices[0]), matrices.size() * sizeof(glm::mat4)));

		BufferLayout layout1 = {
			{ ShaderDataType::Mat4, "aInstanceMatrix" },
		};
		MatrixVB->SetLayout(layout1);

		cube->GetVertexArray()->AddInstancedVertexBuffer(MatrixVB, 4);
	}

	void DebugPoint::DrawUpdatePoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size, PointType type)
	{
		Point point{ position,rotation,color,size,type };

		points.push(point);
	}

	void DebugPoint::AddPoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size, PointType type)
	{
		CubesNumber++;

		colors.push_back(glm::vec4(color,1));

		glm::mat4 rot = glm::toMat4(glm::quat(rotation));

		glm::mat4 model =  glm::translate(glm::mat4(1.0f), position)
			* rot
			* glm::scale(glm::mat4(1.0f), glm::vec3(size));

		matrices.push_back(model);
	}
}