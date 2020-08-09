#pragma once

#ifdef CE_PLATFORM_WINDOWS

extern CrashEngine::Application* CrashEngine::CreateApplication();

int main(int argc,char** argv)
{
	auto app = CrashEngine::CreateApplication();
	app->Run();
	delete app;

}

#endif