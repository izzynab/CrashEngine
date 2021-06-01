#include "cepch.h"
#include "Debugger.h"

namespace CrashEngine {

	Debugger::Debugger()
	{
		
	}

	void Debugger::Begin()
	{
		debugLines.reset(new DebugLine());
		debugLines->AddGrid(60);

		debugPoints.reset(new DebugPoint());
		//debugPoints->Begin();
	}

	Debugger* Debugger::Create()
	{
		return new Debugger();
	}

	void Debugger::OnUpdate(Camera& camera)
	{
		debugLines->OnUpdate(camera);
		debugPoints->OnUpdate(camera);
	}
}