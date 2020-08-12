#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace CrashEngine {
	class CRASH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//to be defined in client
	Application* CreateApplication();
}