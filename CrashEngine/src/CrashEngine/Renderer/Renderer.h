#pragma once

#include "RenderCommand.h"

#include "CameraController.h"

#include "Model.h"

#include "Texture.h"

#include "Shader.h"

#include "BasicShapes.h"

#include "Framebuffer.h"



namespace CrashEngine {

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, bool strip = false);
		static void SubmitDebug(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};


}