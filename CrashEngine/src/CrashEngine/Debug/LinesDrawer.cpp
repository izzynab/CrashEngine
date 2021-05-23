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

		//UpdateLine.reset(new Line());
	}

	void DebugLine::OnUpdate(Camera& camera)
	{
		for (int i = 0; i < lines.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			shader->Bind();
			shader->SetUniformVec3("color", lines[i].color);
			shader->SetUniformMat4("model", model);
			shader->SetUniformVec2("u_viewport_size", glm::vec2(camera.ScreenWidth, camera.ScreenHeight));
			shader->SetUniformVec2("u_aa_radius", glm::vec2(0.5, 0.5));
			shader->SetUniformFloat("lineWidth", lines[i].width);

			Renderer::SubmitLine(lines[i].VAO, 2, lines[i].width);
		}

		for (int i = 0; i < linesSet.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			shader->Bind();
			shader->SetUniformVec3("color", linesSet[i].color);
			shader->SetUniformMat4("model", model);
			shader->SetUniformVec2("u_viewport_size", glm::vec2(camera.ScreenWidth, camera.ScreenHeight));
			shader->SetUniformVec2("u_aa_radius", glm::vec2(0.5, 0.5));
			shader->SetUniformFloat("lineWidth", linesSet[i].width);

			Renderer::SubmitLine(linesSet[i].VAO, linesSet[i].NumberOfLines*2, linesSet[i].width);
		}
	}

	void DebugLine::DrawUpdateLine(glm::vec3 startVec, glm::vec3 endVec, glm::vec3 color, float width)
	{
		UpdateLinesNumber++;

		AddLine(startVec, endVec, color, width);
	}

	void DebugLine::DrawFrustum(Camera& camera)
	{
		//ClearUpdateLines();

		/*float tanHalfHFOV = tanf(glm::radians(camera.fov / 2.0f));
		float tanHalfVFOV = tanf(glm::radians((camera.fov * (camera.ScreenHeight / camera.ScreenWidth)) / 2.0f));

		float xn = 10 * tanHalfHFOV;
		float xf = 1500 * tanHalfHFOV;
		float yn = 10 * tanHalfVFOV;
		float yf = 1500 * tanHalfVFOV;

		glm::vec3 frustumCorners[8] = {
			// near face
			camera.GetViewMatrix() * glm::vec4(glm::vec3(xn, yn, 10) + camera.GetPosition(),1),
			camera.GetViewMatrix()* glm::vec4(glm::vec3(-xn, yn, 10) + camera.GetPosition(),1),
			camera.GetViewMatrix() * glm::vec4(glm::vec3(xn, -yn, 10) + camera.GetPosition(),1),
			camera.GetViewMatrix() * glm::vec4(glm::vec3(-xn, -yn, 10) + camera.GetPosition(),1),

			// far face
			camera.GetViewMatrix()* glm::vec4(glm::vec3(xf, yf,1500) + camera.GetPosition(),1),
			camera.GetViewMatrix() * glm::vec4(glm::vec3(-xf, yf, 1500) + camera.GetPosition(),1),
			camera.GetViewMatrix() * glm::vec4(glm::vec3(xf, -yf, 1500) + camera.GetPosition(),1),
			camera.GetViewMatrix() * glm::vec4(glm::vec3(-xf, -yf, 1500) + camera.GetPosition(),1)
		};*/


		glm::mat4 inv = camera.GetViewMatrix() * camera.GetProjectionMatrix();

		glm::vec4 f[8u] =
		{
			// near face
			{1, 1, 10, 1.f},
			{-1, 1, 10, 1.f},
			{1, -1, 10, 1.f},
			{-1, -1, 10, 1.f},

			// far face
			{1, 1, 1500, 1.f},
			{-1, 1, 1500 , 1.f},
			{1, -1, 1500 , 1.f},
			{-1, -1,1500, 1.f},
		};

		glm::vec3 frustumCorners[8];
		for (int i = 0; i < 8; i++)
		{
			glm::vec4 ff = inv * f[i];
			frustumCorners[i].x = ff.x / ff.w;
			frustumCorners[i].y = ff.y / ff.w;
			frustumCorners[i].z = ff.z / ff.w;
		}


		DrawUpdateLine(frustumCorners[0], frustumCorners[1],glm::vec3(0.8f,0.8f,0.8f),3.f);
		DrawUpdateLine(frustumCorners[2], frustumCorners[3],glm::vec3(0.8f,0.8f,0.8f),3.f);
		DrawUpdateLine(frustumCorners[0], frustumCorners[2],glm::vec3(0.8f,0.8f,0.8f),3.f);
		DrawUpdateLine(frustumCorners[1], frustumCorners[3],glm::vec3(0.8f,0.8f,0.8f),3.f);
														  
		DrawUpdateLine(frustumCorners[4], frustumCorners[5],glm::vec3(0.8f,0.8f,0.8f),3.f);
		DrawUpdateLine(frustumCorners[6], frustumCorners[7],glm::vec3(0.8f,0.8f,0.8f),3.f);
		DrawUpdateLine(frustumCorners[4], frustumCorners[6],glm::vec3(0.8f,0.8f,0.8f),3.f);
		DrawUpdateLine(frustumCorners[5], frustumCorners[7],glm::vec3(0.8f,0.8f,0.8f),3.f);
														 
		DrawUpdateLine(frustumCorners[0], frustumCorners[4],glm::vec3(0.8f,0.8f,0.8f),3.f);
		DrawUpdateLine(frustumCorners[1], frustumCorners[5],glm::vec3(0.8f,0.8f,0.8f),3.f);
		DrawUpdateLine(frustumCorners[2], frustumCorners[6],glm::vec3(0.8f,0.8f,0.8f),3.f);
		DrawUpdateLine(frustumCorners[3], frustumCorners[7],glm::vec3(0.8f,0.8f,0.8f),3.f);
	}

	void DebugLine::ClearUpdateLines()
	{
		lines.erase(lines.end()- UpdateLinesNumber, lines.end());
		UpdateLinesNumber = 0;
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