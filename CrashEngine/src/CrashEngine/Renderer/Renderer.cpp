#include "cepch.h"
#include "Renderer.h"
#include "CrashEngine/Scene/Scene.h"
#include "CrashEngine/Core/Application.h"
#include "Texture.h"
#include "TextureCreator.h"
#include "Shader.h"
#include "BasicShapes.h"
#include "Framebuffer.h"

#include "RenderProperties.h"

namespace CrashEngine {

	void Renderer::BeginScene()
	{
	}

	void Renderer::RenderScene(std::shared_ptr<RenderProperties>& renderProperties, std::shared_ptr<Framebuffer>& renderFramebuffer, Camera* camera, Timestep ts)
	{
		if (camera->ScreenWidth <= 0 || camera->ScreenHeight <= 0)
		{
			CE_CORE_ASSERT("Camera screen size was 0");
			return;
		}


		auto ActiveScene = renderProperties->GetScene();
		auto DefferedFramebuffer = renderProperties->GetDefferedFramebuffer();
		auto UniformBufferObject = renderProperties->GetUBO();
		auto defferedShader = renderProperties->GetDefferedShader();
		auto gBufferShader = renderProperties->GetGBufferShader();

		//----------------shadows----------------
		ActiveScene->directionalLight->DrawCSM(camera, defferedShader.get());
		//----------------shadows----------------

		DefferedFramebuffer->Resize(camera->ScreenWidth, camera->ScreenHeight);
		renderFramebuffer->Resize(camera->ScreenWidth, camera->ScreenHeight);

		glm::mat4 view = camera->GetViewMatrix();
		UniformBufferObject->setData("view", glm::value_ptr(view));

		glm::mat4 projection = glm::perspective(glm::radians(camera->fov), (float)camera->ScreenWidth / (float)camera->ScreenHeight, 0.1f, 1500.0f);
		UniformBufferObject->setData("projection", glm::value_ptr(projection));
		//-----------deffered------------------------

		DefferedFramebuffer->Bind();
		RenderCommand::SetViewport(camera->ScreenWidth, camera->ScreenHeight);
		RenderCommand::SetClearColor({ 1.f, 0.f, 0.0f, 1.0f });
		RenderCommand::Clear();

		ActiveScene->OnUpdate(ts, gBufferShader.get());
		DefferedFramebuffer->Unbind();

		//-------ssao-------
		ActiveScene->ssao->Render(camera->ScreenWidth, camera->ScreenHeight, DefferedFramebuffer->GetColorAttachmentRendererID(0), DefferedFramebuffer->GetColorAttachmentRendererID(2));
		//-------ssao-------

		renderFramebuffer->Bind();
		RenderCommand::SetViewport(camera->ScreenWidth, camera->ScreenHeight);
		RenderCommand::Clear();

		defferedShader->Bind();

		RenderCommand::BindTexture(DefferedFramebuffer->GetColorAttachmentRendererID(0), 0);
		RenderCommand::BindTexture(DefferedFramebuffer->GetColorAttachmentRendererID(1), 1);
		RenderCommand::BindTexture(DefferedFramebuffer->GetColorAttachmentRendererID(2), 2);
		RenderCommand::BindTexture(DefferedFramebuffer->GetColorAttachmentRendererID(3), 3);
		RenderCommand::BindTexture(ActiveScene->ssao->GetTextureRendererID(), 7);

		ActiveScene->skyLight->BindIrradianceMap(4);
		ActiveScene->skyLight->BindPrefilterMap(5);
		ActiveScene->skyLight->BindbrdfTexture(6);

		glm::vec3 rotation = glm::mat3(view) * glm::vec3(
			glm::cos(ActiveScene->directionalLight->rotation.x) * glm::sin(ActiveScene->directionalLight->rotation.y),
			glm::sin(ActiveScene->directionalLight->rotation.x) * glm::sin(ActiveScene->directionalLight->rotation.y),
			glm::cos(ActiveScene->directionalLight->rotation.y));


		//defferedShader->Bind();
		defferedShader->SetUniformVec3("lightRotation", rotation);
		defferedShader->SetUniformVec3("lightColor", ActiveScene->directionalLight->color * ActiveScene->directionalLight->intensity);
		defferedShader->SetUniformVec3("camPos", camera->GetPosition());
		for (int i = 0; i < 3; i++)
			ActiveScene->directionalLight->depthMap[i]->Bind(8 + i);


		//renderFramebuffer->Bind();
		//defferedShader->Bind();
		renderProperties->GetQuad()->RenderQuad();

		DefferedFramebuffer->BlitDepthToFramebuffer(renderFramebuffer);
		//-----------deffered------------------------

		//-----------Post Proscess-------------------
		ActiveScene->postProcess->Blur(renderFramebuffer);

		renderFramebuffer->Bind();
		ActiveScene->skyLight->RenderSky();
		renderFramebuffer->Unbind();

		//-----------Post Proscess-------------------
		ActiveScene->postProcess->ApplyFXAA(renderFramebuffer);
		ActiveScene->postProcess->GammaHDRCorretion(renderFramebuffer);

		//-----------Debug lines---------------------
		renderFramebuffer->Bind();
		//auto& camera = camera;
		Application::Get().GetDebugger().OnUpdate(*camera);
		renderFramebuffer->Unbind();
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

	void Renderer::SubmitLine(const std::shared_ptr<VertexArray>& vertexArray, int count, float width)
	{
		vertexArray->Bind();
		RenderCommand::DrawLine(vertexArray,count, width);
	}

	/*void Renderer::DrawModel(Model* model, Shader* shader)
	{
		model->Draw(shader);
	}*/

}