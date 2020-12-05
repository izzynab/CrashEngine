#include <Crash.h>
#include "CrashEngine/Core/EntryPoint.h"

#include "Layers/EditorLayer.h"

namespace CrashEngine {
	
	class Sandbox : public CrashEngine::Application
	{
	public:
		Sandbox()
		{
			PushLayer(new Editor());
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