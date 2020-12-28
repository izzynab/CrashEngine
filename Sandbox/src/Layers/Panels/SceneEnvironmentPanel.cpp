#include "SceneEnvironmentPanel.h"

#include "CrashEngine/Core/Log.h"
#include "CrashEngine/Utils/PlatformUtils.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>



namespace CrashEngine {
	SceneEnvironmentPanel::SceneEnvironmentPanel(const std::shared_ptr<SkyLight>& skyLight)
		:m_SkyLight(skyLight)
	{

	}

	void SceneEnvironmentPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Environment");

		const ImGuiTreeNodeFlags treeNodeFlags =  ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		bool skybox = ImGui::TreeNodeEx("Skybox", treeNodeFlags);
		if (ImGui::IsItemClicked())
		{

		}

		if (skybox)
		{
			if (ImGui::Button("Load skybox"))
			{
				std::optional<std::string> filepath = FileDialogs::OpenFile("");
				if (filepath)
				{
					m_SkyLight->LoadHDR(filepath.value());
				}
			}
			if (!(m_SkyLight->directory<"  "))
			{
				ImGui::Text(m_SkyLight->directory.c_str());
			}



			ImGui::TreePop();
		}

		bool directional = ImGui::TreeNodeEx("Directional Light", treeNodeFlags);
		if (directional)
		{
			ImGui::TreePop();
		}
		ImGui::End();
	}
}