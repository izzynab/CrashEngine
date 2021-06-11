#include "cepch.h"
#include "LinesDrawer.h"

#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Math/Math.h"
#include "CrashEngine/Core/Defines.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

namespace CrashEngine {
	DebugLine::DebugLine()
	{
		shader = Shader::Create("lines.vert", "lines.frag","lines.geom");
	}

	void DebugLine::OnUpdate(Camera& camera, bool erasePoints)
	{
		for (int i = 0; i < lines.size(); i++)
		{
			shader->Bind();
			shader->SetUniformVec3("color", lines[i].color);
			shader->SetUniformVec2("u_viewport_size", glm::vec2(camera.ScreenWidth, camera.ScreenHeight));
			shader->SetUniformVec2("u_aa_radius", glm::vec2(0, 0));
			shader->SetUniformFloat("lineWidth", lines[i].width);

			Renderer::SubmitLine(lines[i].VAO, 2, lines[i].width);
		}

		int size = updateLines.size();
		for (int i = 0; i < size; i++)
		{
			shader->Bind();
			shader->SetUniformVec3("color", updateLines.front().color);
			shader->SetUniformVec2("u_viewport_size", glm::vec2(camera.ScreenWidth, camera.ScreenHeight));
			shader->SetUniformVec2("u_aa_radius", glm::vec2(0, 0));
			shader->SetUniformFloat("lineWidth", updateLines.front().width);

			Renderer::SubmitLine(updateLines.front().VAO, 2, updateLines.front().width);

			if (erasePoints)updateLines.pop_front();
		}

		for (int i = 0; i < linesSet.size(); i++)
		{
			shader->Bind();
			shader->SetUniformVec3("color", linesSet[i].color);
			shader->SetUniformVec2("u_viewport_size", glm::vec2(camera.ScreenWidth, camera.ScreenHeight));
			shader->SetUniformVec2("u_aa_radius", glm::vec2(0.1, 0.1));
			shader->SetUniformFloat("lineWidth", linesSet[i].width);

			Renderer::SubmitLine(linesSet[i].VAO, linesSet[i].NumberOfLines*2, linesSet[i].width);
		}
	}

	void DebugLine::OnFirstFrame()
	{

	}

	void DebugLine::DrawUpdateLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color, float width)
	{
		Line line;
		line.vertices.push_back(startVec.x);
		line.vertices.push_back(startVec.y);
		line.vertices.push_back(startVec.z);

		line.vertices.push_back(endVec.x);
		line.vertices.push_back(endVec.y);
		line.vertices.push_back(endVec.z);

		line.VAO.reset(VertexArray::Create());
		line.VBO.reset(VertexBuffer::Create(&line.vertices[0], line.vertices.size() * sizeof(float)));

		BufferLayout layout1 = {
			{ ShaderDataType::Float3, "aPosition" },
		};
		line.VBO->SetLayout(layout1);

		line.VAO->AddVertexBuffer(line.VBO);

		line.width = width;
		line.color = color;

		updateLines.push_back(line);
	}

	void DebugLine::DrawFrustum(Camera& camera)
	{
		//glm::mat4 inv = glm::inverse(camera.GetProjectionMatrix() * camera.GetViewMatrix());
		DrawFrustum(camera.GetProjectionMatrix(), camera.GetViewMatrix());
	}

	void DebugLine::DrawFrustum(const glm::mat4& projection,const glm::mat4& view)
	{
		glm::mat4 inv = glm::inverse(projection * view);

		glm::vec4 f[8u] =
		{
			// near face
			{1, 1, -1, 1.f},
			{-1, 1, -1, 1.f},
			{1, -1, -1, 1.f},
			{-1, -1, -1, 1.f},

			// far face
			{1, 1, 1, 1.f},
			{-1, 1, 1 , 1.f},
			{1, -1, 1 , 1.f},
			{-1, -1,1, 1.f},
		};

		glm::vec3 frustumCorners[8];
		for (int i = 0; i < 8; i++)
		{
			glm::vec4 ff = inv * f[i];
			frustumCorners[i].x = ff.x / ff.w;
			frustumCorners[i].y = ff.y / ff.w;
			frustumCorners[i].z = ff.z / ff.w;
		}

		DrawUpdateLine(frustumCorners[0], frustumCorners[1], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
		DrawUpdateLine(frustumCorners[0], frustumCorners[2], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
		DrawUpdateLine(frustumCorners[3], frustumCorners[1], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
		DrawUpdateLine(frustumCorners[3], frustumCorners[2], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);

		DrawUpdateLine(frustumCorners[4], frustumCorners[5], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
		DrawUpdateLine(frustumCorners[4], frustumCorners[6], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
		DrawUpdateLine(frustumCorners[7], frustumCorners[5], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
		DrawUpdateLine(frustumCorners[7], frustumCorners[6], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);

		DrawUpdateLine(frustumCorners[0], frustumCorners[4], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
		DrawUpdateLine(frustumCorners[1], frustumCorners[5], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
		DrawUpdateLine(frustumCorners[3], frustumCorners[7], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
		DrawUpdateLine(frustumCorners[2], frustumCorners[6], glm::vec3(0.8f, 0.8f, 0.8f), 1.f);
	}


	void DebugLine::AddLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color, float width)
	{
		Line line;
		line.vertices.push_back(startVec.x);
		line.vertices.push_back(startVec.y);
		line.vertices.push_back(startVec.z);

		line.vertices.push_back(endVec.x);
		line.vertices.push_back(endVec.y);
		line.vertices.push_back(endVec.z);

		line.VAO.reset(VertexArray::Create());
		line.VBO.reset(VertexBuffer::Create(&line.vertices[0], line.vertices.size() * sizeof(float)));

		BufferLayout layout1 = {
			{ ShaderDataType::Float3, "aPosition" },
		};
		line.VBO->SetLayout(layout1);

		line.VAO->AddVertexBuffer(line.VBO);

		line.width = width;
		line.color = color;
	
		lines.push_back(line);

	}

	void DebugLine::AddGrid(const float gridSize)
	{
		LinesSet smallWidth;
		smallWidth.color = glm::vec3(0.3f, 0.3f, 0.3f);
		smallWidth.width = 1.1f;

		LinesSet ordinaryWidth;
		ordinaryWidth.color = glm::vec3(0.23f, 0.23f, 0.23f);
		ordinaryWidth.width = 1.5f;

		LinesSet bigWidth;
		bigWidth.color = glm::vec3(0.2f, 0.2f, 0.2f);
		bigWidth.width = 3.3f;

		for (int grid = -gridSize; grid <= gridSize; grid += 1.f)
		{
			if (grid % 10 == 0 && grid != 0)
			{
				glm::vec3 startXAxis = glm::vec3(grid, 0.f, -gridSize);
				glm::vec3 endXAxix = glm::vec3(grid, 0.f, gridSize);
				ordinaryWidth.AddLine(startXAxis, endXAxix);

				glm::vec3 startZAxis = glm::vec3(-gridSize, 0.f, grid);
				glm::vec3 endZAxix = glm::vec3(gridSize, 0.f, grid);
				ordinaryWidth.AddLine(startZAxis, endZAxix);
			}
			else if (grid == 0)
			{
				glm::vec3 startXAxis = glm::vec3(grid, 0.f, -gridSize);
				glm::vec3 endXAxix = glm::vec3(grid, 0.f, gridSize);
				bigWidth.AddLine(startXAxis, endXAxix);

				glm::vec3 startZAxis = glm::vec3(-gridSize, 0.f, grid);
				glm::vec3 endZAxix = glm::vec3(gridSize, 0.f, grid);
				bigWidth.AddLine(startZAxis, endZAxix);
			}
			else
			{
				glm::vec3 startXAxis = glm::vec3(grid, 0.f, -gridSize);
				glm::vec3 endXAxix = glm::vec3(grid, 0.f, gridSize);
				smallWidth.AddLine(startXAxis, endXAxix);

				glm::vec3 startZAxis = glm::vec3(-gridSize, 0.f, grid);
				glm::vec3 endZAxix = glm::vec3(gridSize, 0.f, grid);
				smallWidth.AddLine(startZAxis, endZAxix);
			}	
			
		}

		linesSet.push_back(smallWidth);
		linesSet.push_back(ordinaryWidth);
		linesSet.push_back(bigWidth);
	}
}