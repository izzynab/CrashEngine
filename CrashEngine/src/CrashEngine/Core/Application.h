#pragma once

#include "Core.h"
#include "CrashEngine/Events/Event.h"
#include "Window.h"
#include "CrashEngine/Events/ApplicationEvent.h"

#include "CrashEngine/Core/LayerStack.h"
#include "CrashEngine/Events/Event.h"
#include "CrashEngine/Events/ApplicationEvent.h"

#include "CrashEngine/ImGui/ImGuiLayer.h"

#include "CrashEngine/Debug/Debugger.h"


namespace CrashEngine {
	class CRASH_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline Debugger& GetDebugger() { return *m_Debugger; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::unique_ptr<Debugger> m_Debugger;

		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	//to be defined in client
	Application* CreateApplication();
}