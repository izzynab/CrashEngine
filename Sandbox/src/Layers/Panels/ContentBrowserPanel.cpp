#include "cepch.h"
#include "ContentBrowserPanel.h"

#include "CrashEngine/Core/Log.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <filesystem>

namespace CrashEngine {

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		ImGui::End();
	}
}