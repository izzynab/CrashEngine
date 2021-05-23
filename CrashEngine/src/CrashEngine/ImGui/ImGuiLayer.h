#pragma once

#include "CrashEngine/Core/Layer.h"

#include "CrashEngine/Events/ApplicationEvent.h"
#include "CrashEngine/Events/KeyEvent.h"
#include "CrashEngine/Events/MouseEvent.h"

#include "CrashEngine/Renderer/Framebuffer.h"
#include "CrashEngine/Renderer/RenderProperties.h"
#include "imgui.h"

//class RenderProperties;

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

		void Dockspace(std::shared_ptr<RenderProperties>& renderProperties ,std::vector<std::shared_ptr<Framebuffer>> framebuffers);
		void MainMenu();
		void StyleEditor();
		void Menu();
		void WindowMetrics();


	public:
		//ImVec2 CurrentWindowView = ImVec2(0, 0);
		//ImVec2 OldWindowSize = ImVec2(0, 0);

		bool EditorStyleEnabled = false;
		bool MenuEnabled = false;
		bool WindowMetricsEnabled = false;
	private:
		float m_Time = 0.0f;

	};

}
