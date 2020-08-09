#include <Crash.h>

class Sandbox : public CrashEngine::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

CrashEngine::Application* CrashEngine::CreateApplication()
{
	return new Sandbox;
}