#pragma once

#include "CrashEngine/Layer.h"

#include "CrashEngine/Events/ApplicationEvent.h"
#include "CrashEngine/Events/KeyEvent.h"
#include "CrashEngine/Events/MouseEvent.h"

#include "CrashEngine/Renderer/Framebuffer.h"
#include "imgui.h"


namespace CrashEngine {

	class CRASH_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void Dockspace(std::shared_ptr<Framebuffer> renderFramebuffer);

		void MainMenu();

		void StyleEditor();

		void Menu();

		void WindowMetrics();


	public:
		ImVec2 CurrentWindowView = ImVec2(0, 0);
		ImVec2 OldWindowSize = ImVec2(0, 0);

		bool EditorStyleEnabled = false;
		bool MenuEnabled = true;
		bool WindowMetricsEnabled = false;
	private:
		float m_Time = 0.0f;

	};

}
