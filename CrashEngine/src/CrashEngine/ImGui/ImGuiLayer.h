#pragma once

#include "CrashEngine/Core/Layer.h"

#include "CrashEngine/Events/ApplicationEvent.h"
#include "CrashEngine/Events/KeyEvent.h"
#include "CrashEngine/Events/MouseEvent.h"
#include "CrashEngine/Events/WindowEvent.h"

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

	public:


	};

}
