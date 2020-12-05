#pragma once

#ifdef CE_PLATFORM_WINDOWS

//todo: this extern create link problem when is linked 
extern CrashEngine::Application* CrashEngine::CreateApplication();

int main(int argc, char** argv)
{
	CrashEngine::Log::Init();
	CE_CORE_WARN("Hello there");

	auto app = CrashEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif