#include "SceneEnvironmentPanel.h"

#include "CrashEngine/Core/Log.h"
#include "CrashEngine/Utils/PlatformUtils.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>



namespace CrashEngine {
	SceneEnvironmentPanel::SceneEnvironmentPanel(const std::shared_ptr<SkyLight>& skyLight, const std::shared_ptr<DirectionalLight>& directionalLight)
		:m_SkyLight(skyLight),m_DirectionalLight(directionalLight)
	{
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
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

			if (ImGui::Button("Delete skybox"))
			{
				m_SkyLight->LoadHDR(" ");
			}



			ImGui::TreePop();
		}

		bool directional = ImGui::TreeNodeEx("Directional Light", treeNodeFlags);
		if (directional)
		{
			DrawVec3Control("Position", m_DirectionalLight->position);

			glm::vec3 rotation = glm::degrees(m_DirectionalLight->rotation);
			DrawVec3Control("Rotation", rotation);
			m_DirectionalLight->rotation = glm::radians(rotation);

			ImGui::NewLine();
			ImGui::ColorPicker3("Light color", &m_DirectionalLight->color.x);

			ImGui::NewLine();
			ImGui::SliderFloat("Intensity", &m_DirectionalLight->intensity, 0.f, 150.f);

			ImGui::TreePop();
		}
		ImGui::End();
	}

	

}