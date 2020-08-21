#pragma once

#include "CrashEngine/Layer.h"

#include "CrashEngine/Events/ApplicationEvent.h"
#include "CrashEngine/Events/KeyEvent.h"
#include "CrashEngine/Events/MouseEvent.h"

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

	private:
		float m_Time = 0.0f;
	};

}
