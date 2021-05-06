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
		//debugLines->AddLine(glm::vec3(0, 0, 0), glm::vec3(20, 20, 20), glm::vec3(0, 0, 0.7f), 5);
	}

	Debugger* Debugger::Create()
	{
		return new Debugger();
	}

	void Debugger::OnUpdate(Camera& camera)
	{
		debugLines->OnUpdate(camera);
	}
}