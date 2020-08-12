#pragma once

#ifdef CE_PLATFORM_WINDOWS
#ifdef CE_BUILD_DLL
#define CRASH_API __declspec(dllexport)
#else
#define CRASH_API  __declspec(dllimport)
#endif
#else
#error Crash engine only supports windows!
#endif

#define BIT(x) (1 << x) 