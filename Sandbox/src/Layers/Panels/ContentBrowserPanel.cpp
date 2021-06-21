#include "cepch.h"
#include "ContentBrowserPanel.h"

#include "CrashEngine/Core/Log.h"

#include "IconsFontAwesome5.h"
#include "glm/glm.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace CrashEngine {

	static const std::filesystem::path m_AssetPath = "..\\Content";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(m_AssetPath)
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (ImGui::GetWindowWidth() < 120)
		{
			ImGui::End();
			return;
		}

		int columnSize = (int)(ImGui::GetWindowWidth()/ 120);

		ImGuiTableFlags flags = ImGuiTableFlags_None;

		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 10.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
		ImGui::BeginTable("Table", columnSize, flags);
		ImGui::PopStyleVar(2);

		if (m_CurrentDirectory != m_AssetPath)
		{
			ImGui::TableNextColumn();
			if (IconButton(ICON_FA_LONG_ARROW_ALT_LEFT))//todo: instead create hierarchy od files 
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for (auto& it : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			if (it.is_directory())
			{
				ImGui::TableNextColumn();

				if(IconButton(ICON_FA_FOLDER_OPEN, it.path().filename().string().c_str()))
				{
					m_CurrentDirectory /= it.path().filename();		
				}
			}		
			else
			{
				ImGui::TableNextColumn();
				std::string extension = it.path().extension().string();
				if (extension == ".obj" || extension == ".fbx") IconButton(ICON_FA_CUBE, it.path().filename().string().c_str(),0.85f);
				if (extension == ".crash") IconButton(ICON_FA_ARCHWAY, it.path().filename().string().c_str(),0.7f);
				if (extension == ".png" || extension == ".jpg" || extension == ".tga");
			}
		}
		ImGui::EndTable();
		ImGui::End();
	}

	bool ContentBrowserPanel::IconButton(const char* icon, const char* filename, float scale)
	{
		ImGuiButtonFlags flags = ImGuiButtonFlags_None;

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		ImGuiID id;
		if(filename)id = window->GetID(filename);
		else id = window->GetID(icon);


		glm::vec2 pos = window->DC.CursorPos;
		if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
			pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
		glm::vec2 size = { 100,100 };

		const ImRect bb(pos, pos + size);
		ImGui::ItemSize(size, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		if (g.CurrentItemFlags & ImGuiItemFlags_ButtonRepeat)
			flags |= ImGuiButtonFlags_Repeat;
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

		// Render
		ImU32 col;
		if (held && hovered) col = ImGui::GetColorU32(ImGuiCol_Button);
		else if (hovered) col = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
		else col = ImGui::GetColorU32({0,0,0,0});

		ImGui::RenderNavHighlight(bb, id);
		ImGui::RenderFrame(bb.Min, bb.Max, col, false, 7.f);

		ImFont* font = ImGui::GetFont();
		font->Scale = scale;
		ImGui::PushFont(font);

		const ImVec2 icon_size = ImGui::CalcTextSize(icon, NULL, true);
		ImGui::RenderTextClipped(bb.Min, bb.Max, icon, NULL, &icon_size, { (1.f - scale)*2 , (scale - 1.f)/2 + 0.8f}, &bb);

		font->Scale = 1.f;
		ImGui::PopFont();

		if (filename == nullptr) return pressed;

		const ImVec2 filename_size = ImGui::CalcTextSize(filename, NULL, true);
		ImGui::RenderTextClipped(bb.Min, bb.Max, filename, NULL, &filename_size, { 0.5f,1.0f }, &bb);

		return pressed;

	}
}