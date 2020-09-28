#pragma once

#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"

namespace CrashEngine {

	class Sphere
	{
	public:
		Sphere();

		void RenderSphere();

	private:
		unsigned int sphereVAO = 0;
		unsigned int indexCount = 0;

		std::shared_ptr<VertexArray> SphereVA;
		std::shared_ptr<IndexBuffer> SphereIB;
	};

	class Square
	{
		Square();

		void RenderSquare();

	private:
		unsigned int squareVAO = 0;
		unsigned int indexCount = 0;

		std::shared_ptr<VertexArray> SquareVA;
		std::shared_ptr<IndexBuffer> SquareIB;
	};
}
