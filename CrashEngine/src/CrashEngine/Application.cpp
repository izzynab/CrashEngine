#include "cepch.h"

#include "Application.h"

#include "CrashEngine/Events/ApplicationEvent.h"
#include "CrashEngine/Log.h"

namespace CrashEngine {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			CE_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			CE_TRACE(e);
		}

		e.GetCategoryFlags();

		while (true);
	}
}