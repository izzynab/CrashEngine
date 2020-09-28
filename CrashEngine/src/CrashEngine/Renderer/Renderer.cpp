#include "cepch.h"
#include "Renderer.h"

namespace CrashEngine {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::SubmitDebug(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawTriangles(vertexArray);
	}

}