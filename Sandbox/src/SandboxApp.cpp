#include <Crash.h>
#include "EditorLayer.h"


namespace CrashEngine {
	
	class Sandbox : public CrashEngine::Application
	{
	public:
		Sandbox()
		{
			PushLayer(new EditorLayer());
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