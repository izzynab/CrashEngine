#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "CrashEngine/Events/ApplicationEvent.h"

#include "CrashEngine/LayerStack.h"
#include "CrashEngine/Events/Event.h"
#include "CrashEngine/Events/ApplicationEvent.h"

#include "CrashEngine/ImGui/ImGuiLayer.h"


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

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

	//to be defined in client
	Application* CreateApplication();
}