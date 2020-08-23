#include <Crash.h>

#include "imgui/imgui.h"

#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Renderer/Camera.h"

#include <GLFW/include/GLFW/glfw3.h>


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


			std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = projection * view * model * vec4(a_Position, 1.0);
			}
		)";

			std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

			m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

			//camera.reset(new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

			m_BlueShader->Bind();

			model = glm::mat4(1.0f);
			model = glm::rotate(model, 30.f, glm::vec3(1.f, 1.0f, 1.0f));
			m_BlueShader->SetUniformMat4("model", model);


			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			m_BlueShader->SetUniformMat4("view", view);


			projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
			m_BlueShader->SetUniformMat4("projection", projection);

		}

		void OnUpdate() override
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

		
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene();

			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			m_BlueShader->SetUniformMat4("view", view);

			Renderer::SubmitDebug(m_SquareVA, 36);



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
				float cameraSpeed = 2.5f * deltaTime;

				CrashEngine::KeyPressedEvent& e = (CrashEngine::KeyPressedEvent&)event;
				if (e.GetKeyCode() == CE_KEY_W)
				{
					CE_TRACE("w");
					cameraPos += cameraSpeed * cameraFront;
				}

				if (e.GetKeyCode() == CE_KEY_S)
				{
					CE_TRACE("s");
					cameraPos -= cameraSpeed * cameraFront;
				}

				if (e.GetKeyCode() == CE_KEY_A)
				{
					CE_TRACE("a");
					cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
				}

				if (e.GetKeyCode() == CE_KEY_D)
				{
					CE_TRACE("d");
					cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
				}
			}


			if (event.GetEventType() == CrashEngine::EventType::MouseMoved)
			{
				CrashEngine::MouseMovedEvent& e = (CrashEngine::MouseMovedEvent&)event;

				float xpos = e.GetX();
				float ypos = e.GetY();

				if (firstMouse)
				{
					lastX = xpos;
					lastY = ypos;
					firstMouse = false;
				}

				float xoffset = xpos - lastX;
				float yoffset = lastY - ypos;
				lastX = xpos;
				lastY = ypos;

				float sensitivity = 0.1f;
				xoffset *= sensitivity;
				yoffset *= sensitivity;

				yaw += xoffset;
				pitch += yoffset;

				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;

				glm::vec3 direction;
				direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
				direction.y = sin(glm::radians(pitch));
				direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
				cameraFront = glm::normalize(direction);
			}

		}

	private:
		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		std::shared_ptr<Camera> camera;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;


		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame

		//camera
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f,-1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 direction;
		float yaw = -90.0f;
		float pitch = 0.f;

		float lastX = 400, lastY = 300;
		float xoffset;
		float yoffset; // reversed since y-coordinates range from bottom to top

		const float sensitivity = 0.1f;

		bool firstMouse = true;


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