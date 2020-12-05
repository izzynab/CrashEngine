#pragma once

#ifdef CE_PLATFORM_WINDOWS
#if CE_DYNAMIC_LINK
	#ifdef CE_BUILD_DLL
		#define CRASH_API __declspec(dllexport)
	#else
		#define CRASH_API  __declspec(dllimport)
	#endif
#else
	#define CRASH_API
#endif
#else
#error Crash engine only supports windows!
#endif

#ifdef CE_ENABLE_ASSERTS
#define CE_ASSERT(x, ...) { if(!(x)) { CE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define CE_CORE_ASSERT(x, ...) { if(!(x)) { CE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define CE_ASSERT(x, ...)
#define CE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define CE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) 