
#include <Crash.h>

#include "imgui/imgui.h"

#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Renderer/Texture.h"

#include "CrashEngine/Renderer/Model.h"
#include "CrashEngine/Renderer/BasicShapes.h"

#include "CrashEngine/Input.h"
#include "CrashEngine/Window.h"
#include "CrashEngine/FileManager.h"
#include "CrashEngine/Renderer/TextRenderer.h"

#include "CrashEngine/Application.h"

namespace CrashEngine {
	class ExampleLayer : public CrashEngine::Layer
	{
	public:
		ExampleLayer()
			: Layer("Example")
		{
			//auto window = Application::Get().GetWindow();

			float Height = Application::Get().GetWindow().GetHeight();
			float Width = Application::Get().GetWindow().GetWidth();

			camera.reset(new Camera(glm::vec3(0.0f, 0.0f, 3.0f), Width, Height));
			camera->CameraSpeed = 4.f;

			pbrTextureShader = Shader::Create("pbr.vert", "pbrTexture.frag");
			pbrTextureShader->Bind();

			projection = glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 100.0f);
			pbrTextureShader->SetUniformMat4("projection", projection);

			pbrTextureShader->SetUniformInt("albedoMap", 0);
			pbrTextureShader->SetUniformInt("normalMap", 1);
			pbrTextureShader->SetUniformInt("metallicMap", 2);
			pbrTextureShader->SetUniformInt("roughnessMap", 3);
			pbrTextureShader->SetUniformInt("aoMap", 4);

			albedo = Texture2D::Create("cerberus\\Cerberus_A.tga");
			normal = Texture2D::Create("cerberus\\Cerberus_N.tga");
			metallic = Texture2D::Create("cerberus\\Cerberus_M.tga");
			roughness = Texture2D::Create("cerberus\\Cerberus_R.tga");
			ao = Texture2D::Create("cerberus\\Cerberus_AO.tga");


			//gun.reset(Model::Create("C:\\EngineDev\\CrashEngine\\Models\\cerberus\\cerberus.obj", TextureType::TGA));


			sphere.reset(new Sphere());


			textRenderer.reset(TextRenderer::Create());
			RenderCommand::Enable(CE_DEPTH_TEST);

		}

		void OnUpdate() override
		{
			camera->Update();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene();

			view = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);

			pbrTextureShader->Bind();
			pbrTextureShader->SetUniformMat4("view", view);
			pbrTextureShader->SetUniformVec3("camPos", camera->Position);

			//uint32_t albedoID
			RenderCommand::BindTexture(albedo->GetRendererID(), 0);
			RenderCommand::BindTexture(normal->GetRendererID(), 1);
			RenderCommand::BindTexture(metallic->GetRendererID(), 2);
			RenderCommand::BindTexture(roughness->GetRendererID(), 3);
			RenderCommand::BindTexture(ao->GetRendererID(), 4);


			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(1, 1, 1));
			pbrTextureShader->SetUniformMat4("model", model);
			//gun->Draw(pbrTextureShader);

			// render rows*column number of spheres with material properties defined by textures (they all have the same material properties)
			glm::mat4 model = glm::mat4(1.0f);
			for (int row = 0; row < nrRows; ++row)
			{
				for (int col = 0; col < nrColumns; ++col)
				{
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(
						(float)(col - (nrColumns / 2)) * spacing,
						(float)(row - (nrRows / 2)) * spacing,
						0.0f
					));
					pbrTextureShader->SetUniformMat4("model", model);
					sphere->RenderSphere();
				}
			}

			// render light source (simply re-render sphere at light positions)
				   // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
				   // keeps the codeprint small.
			for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
			{
				glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(RenderCommand::GetTime() * 5.0) * 5.0, 0.0, 0.0);
				newPos = lightPositions[i];
				pbrTextureShader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
				pbrTextureShader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

				model = glm::mat4(1.0f);
				model = glm::translate(model, newPos);
				model = glm::scale(model, glm::vec3(0.5f));
				pbrTextureShader->SetUniformMat4("model", model);
				//sphere->RenderSphere();
			}




			//FPS counter
			double currentTime = RenderCommand::GetTime();
			double fps1 = currentTime - lastTime;
			lastTime = currentTime;

			int fps2 = 1/fps1;
			std::string fps = std::to_string(fps2);
			textRenderer->RenderText(fps, 0.0f, 0.0f, 1.0f, glm::vec3(1, 1, 1));

			Renderer::EndScene();
		}

		virtual void OnImGuiRender() override
		{

		}

		void OnEvent(CrashEngine::Event& event) override
		{
			if (event.GetEventType() == CrashEngine::EventType::WindowResize)
			{
				CrashEngine::WindowResizeEvent& e = (CrashEngine::WindowResizeEvent&)event;
				camera->SetHeight(e.GetHeight());
				camera->SetWidth(e.GetWidth());

				projection = glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / (float)e.GetHeight(), 0.1f, 100.0f);
				pbrTextureShader->Bind();
				pbrTextureShader->SetUniformMat4("projection", projection);
				RenderCommand::SetViewport(e.GetWidth(), e.GetHeight());
			}

			if (event.GetEventType() == CrashEngine::EventType::MouseMoved)
			{
				CrashEngine::MouseMovedEvent& e = (CrashEngine::MouseMovedEvent&)event;

			}

		}

	private:
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Model> backpack;
		std::shared_ptr<Model> gun;
		std::shared_ptr<TextRenderer> textRenderer;

		Shader* pbrShader;
		Shader* pbrTextureShader;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		double lastTime = 0;
	
		// lights
		glm::vec3 lightPositions[12] = {
			glm::vec3(-10.0f,  10.0f, 10.0f),
			glm::vec3(10.0f,  10.0f, 10.0f),
			glm::vec3(-10.0f, -10.0f, 10.0f),
			glm::vec3(10.0f, -10.0f, 10.0f),
		};
		glm::vec3 lightColors[12] = {
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f)
		};
		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;

		std::shared_ptr<Sphere> sphere;

		std::shared_ptr<Texture2D> albedo;
		std::shared_ptr<Texture2D> normal;
		std::shared_ptr<Texture2D> metallic;
		std::shared_ptr<Texture2D> roughness;
		std::shared_ptr<Texture2D> ao;
	};

	class Sandbox : public CrashEngine::Application
	{
	public:
		Sandbox()
		{
			PushLayer(new ExampleLayer());

		}
		~Sandbox()
		{
		}
	};

	CrashEngine::Application* CrashEngine::CreateApplication()
	{
		return new Sandbox;
	}
}