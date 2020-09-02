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

//TEMPORARY
#include "GLFW/include/GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



namespace CrashEngine {
	class ExampleLayer : public CrashEngine::Layer
	{
	public:
		ExampleLayer()
			: Layer("Example")
		{
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

			camera.reset(new Camera(glm::vec3(0.0f, 0.0f, 3.0f), 800, 400));

			m_BlueShader->Bind();

			model = glm::mat4(1.0f);
			model = glm::rotate(model, 30.f, glm::vec3(1.f, 1.0f, 1.0f));
			m_BlueShader->SetUniformMat4("model", model);

			view = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
			m_BlueShader->SetUniformMat4("view", view);

			projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
			m_BlueShader->SetUniformMat4("projection", projection);

			backpack.reset(Model::Create("C:\\EngineDev\\CrashEngine\\Models\\backpack\\backpack.obj", false));
			glEnable(GL_DEPTH_TEST);

		}

		void OnUpdate() override
		{
			//TODO: get time func must not be glfw but crash engine api
			float currentFrame = glfwGetTime();
			camera->deltaTime = currentFrame - camera->lastFrame;
			camera->lastFrame = currentFrame;



			float cameraSpeed = 2.5f * camera->deltaTime;
			if (Input::IsMouseButtonPressed(CE_MOUSE_BUTTON_RIGHT))
			{
				if (Input::IsKeyPressed(CE_KEY_W))
				{
					camera->Position += cameraSpeed * camera->Front;
				}
				if (Input::IsKeyPressed(CE_KEY_S))
				{
					camera->Position -= cameraSpeed * camera->Front;
				}
				if (Input::IsKeyPressed(CE_KEY_A))
				{
					camera->Position -= glm::normalize(glm::cross(camera->Front, camera->Up)) * cameraSpeed;
				}
				if (Input::IsKeyPressed(CE_KEY_D))
				{
					camera->Position += glm::normalize(glm::cross(camera->Front, camera->Up)) * cameraSpeed;
				}

				float xpos = Input::GetMouseX();
				float ypos = Input::GetMouseY();

				camera->ChangeDirection(xpos, ypos);
			}
			else
			{
				camera->ResetMousePos();
			}




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