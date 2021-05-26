#pragma once

#include "Event.h"


namespace CrashEngine {

	class CRASH_API WindowFocusEvent : public Event
	{
	public:
		virtual const char* GetName() const override
		{
			return name.c_str();
		}
	private:
		std::string name;
	};

	class CRASH_API WindowLostFocusEvent : public Event
	{
	public:
		virtual const char* GetName() const override
		{
			return name.c_str();
		}
	private:
		std::string name;
	};


}