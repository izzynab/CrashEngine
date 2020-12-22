#include "cepch.h"
#include "Renderer.h"

namespace CrashEngine {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, bool strip)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray, strip);
	}

	void Renderer::SubmitDebug(const std::shared_ptr<VertexArray>& vertexArray, unsigned int trianglesNumber)
	{
		vertexArray->Bind();
		RenderCommand::DrawTriangles(vertexArray, trianglesNumber);
	}

	/*void Renderer::DrawModel(Model* model, Shader* shader)
	{
		model->Draw(shader);
	}*/

}