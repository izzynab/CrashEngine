#include <Crash.h>

#include "imgui/imgui.h"

class ExampleLayer : public CrashEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{

		if (CrashEngine::Input::IsKeyPressed(CE_KEY_TAB))
			CE_TRACE("Tab key is pressed (poll)!");
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
			CrashEngine::KeyPressedEvent& e = (CrashEngine::KeyPressedEvent&)event;
			if (e.GetKeyCode() == CE_KEY_TAB)
				CE_TRACE("Tab key is pressed (event)!");
			CE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

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