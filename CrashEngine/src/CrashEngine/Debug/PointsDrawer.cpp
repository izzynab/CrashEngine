#include "cepch.h"
#include "PointsDrawer.h"

#include "CrashEngine/Renderer/Buffer.h"

namespace CrashEngine {
	DebugPoint::DebugPoint()
	{
		instancedShader = Shader::Create("Instanced3D.vert", "Instanced3D.frag");
		shader = Shader::Create("Basic3D.vert", "Basic3D.frag");
		cube.reset(new Cube());
		sphere.reset(new Sphere());	
	}

	void DebugPoint::OnUpdate(Camera& camera)
	{			
		instancedShader->Bind();
		cube->RenderInstancedCube(cubeDetails.Number);
		//sphere->RenderInstancedSphere(sphereDetails.Number);//todo: not working

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
			switch (points.front().type)
			{
			case PointType::Cube:
				cube->RenderCube();
				break;
			case PointType::Sphere:
				sphere->RenderSphere();
				break;
			}

			points.pop();
		}
	}

	void DebugPoint::OnFirstFrame()
	{
		if (!(cubeDetails.colors.empty() || cubeDetails.matrices.empty()))
		{
			std::shared_ptr<VertexBuffer> ColorVB;
			ColorVB.reset(VertexBuffer::Create(glm::value_ptr(cubeDetails.colors[0]), cubeDetails.colors.size() * sizeof(glm::vec4)));

			BufferLayout layout2 = {
				{ ShaderDataType::Float4, "aColor"},
			};
			ColorVB->SetLayout(layout2);

			cube->GetVertexArray()->AddInstancedVertexBuffer(ColorVB, 3);

			std::shared_ptr<VertexBuffer> MatrixVB;
			MatrixVB.reset(VertexBuffer::Create(glm::value_ptr(cubeDetails.matrices[0]), cubeDetails.matrices.size() * sizeof(glm::mat4)));

			BufferLayout layout1 = {
				{ ShaderDataType::Mat4, "aInstanceMatrix" },
			};
			MatrixVB->SetLayout(layout1);

			cube->GetVertexArray()->AddInstancedVertexBuffer(MatrixVB, 4);
		}
		

		if (!(sphereDetails.colors.empty() || sphereDetails.matrices.empty()))
		{
			std::shared_ptr<VertexBuffer> ColorVB;
			ColorVB.reset(VertexBuffer::Create(glm::value_ptr(sphereDetails.colors[0]), sphereDetails.colors.size() * sizeof(glm::vec4)));

			BufferLayout layout2 = {
				{ ShaderDataType::Float4, "aColor"},
			};
			ColorVB->SetLayout(layout2);

			sphere->GetVertexArray()->AddInstancedVertexBuffer(ColorVB, 3);

			std::shared_ptr<VertexBuffer> MatrixVB;
			MatrixVB.reset(VertexBuffer::Create(glm::value_ptr(sphereDetails.matrices[0]), sphereDetails.matrices.size() * sizeof(glm::mat4)));

			BufferLayout layout1 = {
				{ ShaderDataType::Mat4, "aInstanceMatrix" },
			};
			MatrixVB->SetLayout(layout1);

			sphere->GetVertexArray()->AddInstancedVertexBuffer(MatrixVB, 4);
		}
	}

	void DebugPoint::DrawUpdatePoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size, PointType type)
	{
		Point point{ position,rotation,color,size,type };

		points.push(point);
	}

	void DebugPoint::AddPoint(glm::vec3 position, glm::vec3 rotation, glm::vec3 color, float size, PointType type)
	{
		switch (type)
		{
		case PointType::Cube:
			cubeDetails.Number++;

			cubeDetails.colors.push_back(glm::vec4(color, 1));

			glm::mat4 rot = glm::toMat4(glm::quat(rotation));

			glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
				* rot
				* glm::scale(glm::mat4(1.0f), glm::vec3(size));

			cubeDetails.matrices.push_back(model);
			break;
		case PointType::Sphere:
			sphereDetails.Number++;

			sphereDetails.colors.push_back(glm::vec4(color, 1));

			glm::mat4 rots = glm::toMat4(glm::quat(rotation));

			glm::mat4 models = glm::translate(glm::mat4(1.0f), position)
				* rots
				* glm::scale(glm::mat4(1.0f), glm::vec3(size));

			sphereDetails.matrices.push_back(models);
			break;
		}

	}
}