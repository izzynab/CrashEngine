#include <Crash.h>

#include "imgui/imgui.h"

#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Renderer/Camera.h"

#include "CrashEngine/Renderer/Model.h"

#include "CrashEngine/Input.h"
#include "CrashEngine/Window.h"


namespace CrashEngine {
	class ExampleLayer : public CrashEngine::Layer
	{
	public:
		ExampleLayer()
			: Layer("Example")
		{
			camera.reset(new Camera(glm::vec3(0.0f, 0.0f, 3.0f), 800, 400));


			m_SquareVA.reset(VertexArray::Create());

			float vertices[] = {
			-0.5f, -0.5f, -0.5f, 
			 0.5f, -0.5f, -0.5f, 
			 0.5f,  0.5f, -0.5f, 
			 0.5f,  0.5f, -0.5f, 
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			};

			std::shared_ptr<VertexBuffer> squareVB;
			squareVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

			BufferLayout layout1 = {
				{ ShaderDataType::Float3, "a_Position" }
			};
			squareVB->SetLayout(layout1);


			m_SquareVA->AddVertexBuffer(squareVB);

			//m_BlueShader.reset(new Shader("Basic.vert", "Basic.frag"));
			m_BlueShader = Shader::Create("Basic.vert", "Basic.frag");

			m_BlueShader->Bind();

			model = glm::mat4(1.0f);
			model = glm::rotate(model, 30.f, glm::vec3(1.f, 1.0f, 1.0f));
			m_BlueShader->SetUniformMat4("model", model);

			view = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
			m_BlueShader->SetUniformMat4("view", view);

			projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
			m_BlueShader->SetUniformMat4("projection", projection);

			backpack.reset(Model::Create("C:\\EngineDev\\CrashEngine\\Models\\backpack\\backpack.obj", false));
			RenderCommand::Enable(CE_DEPTH_TEST);

		}

		void OnUpdate() override
		{
			camera->Update();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_BlueShader->Bind();
			view = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
			m_BlueShader->SetUniformMat4("view", view);

			backpack->Draw(m_BlueShader);

			//Renderer::SubmitDebug(m_SquareVA, 36);


			Renderer::EndScene();
		}

		virtual void OnImGuiRender() override
		{
			ImGui::Begin("Test");
			ImGui::Text("Hello World");
			ImGui::End();
		}

		void OnEvent(CrashEngine::Event& event) override
		{
			if (event.GetEventType() == CrashEngine::EventType::KeyPressed)
			{

			}


			if (event.GetEventType() == CrashEngine::EventType::MouseMoved)
			{
				CrashEngine::MouseMovedEvent& e = (CrashEngine::MouseMovedEvent&)event;

			}

		}

	private:
		//std::shared_ptr<Shader> m_BlueShader;
		Shader* m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Model> backpack;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;



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