#include "SceneHierarchyPanel.h"

#include "CrashEngine/Core/Log.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "CrashEngine/Scene/Components.h"
#include <cstring>

#include "CrashEngine/Renderer/Model.h"
#include "CrashEngine/Utils/PlatformUtils.h"
#include "CrashEngine/Renderer/Texture.h"

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace CrashEngine {

	SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const std::shared_ptr<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID , m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, "Components");
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
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

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar(
			);
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				/*if (!m_SelectionContext.HasComponent<CameraComponent>())
					m_SelectionContext.AddComponent<CameraComponent>();
				else
					CE_CORE_ERROR("This entity already has the Camera Component!");*/
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Mesh Component"))
			{
				if (!m_SelectionContext.HasComponent<MeshComponent>())
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{			
						Model* model = new Model(filepath.value());
						m_SelectionContext.AddComponent<MeshComponent>(model);
						m_SelectionContext.GetComponent<MeshComponent>().path = filepath.value();
					}
				}
				else
					CE_CORE_ERROR("This entity already has the Sprite Renderer Component!");
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<MeshComponent>("Mesh Component", entity, [](auto& component)
			{
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100);
				ImGui::Text("File Path:");
				if (ImGui::SmallButton("Change"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						component.path = filepath.value();

						Model* model = new Model(filepath.value());
						component.model = model;
					}
				}
				ImGui::NextColumn();
				ImGui::Text(component.path.c_str());
				ImGui::EndColumns();



				ImGui::NewLine();
				ImGui::Separator();
				ImGui::Text("Albedo Texture");
				if (component.albedo) ImGui::Image((void*)component.albedo->GetRendererID(), ImVec2(150, 150));
				ImGui::SameLine();
				ImGui::Checkbox("Use albedo", &component.albedoTextureUse);

				
				if (ImGui::Button("Open albedo"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						component.albedo = Texture2D::Create(filepath.value());
					}
				}

				if (!(&component.albedoTextureUse))
				{
					ImGui::ColorPicker4("Albedo color", &component.albedoColor.x);
				}

				ImGui::Separator();
				ImGui::Text("Normal Texture");
				
				if (component.normal) ImGui::Image((void*)component.normal->GetRendererID(), ImVec2(150, 150));
				ImGui::SameLine();

				if (ImGui::Button("Open normal"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						component.normal = Texture2D::Create(filepath.value());
					}
				}



				ImGui::Separator();
				ImGui::Text("metallic Texture");

				if (component.metallic) ImGui::Image((void*)component.metallic->GetRendererID(), ImVec2(150, 150));
				ImGui::SameLine();
				ImGui::Checkbox("Use metallic", &component.metallicTextureUse);

				if (ImGui::Button("Open metallic"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						component.metallic = Texture2D::Create(filepath.value());
					}
				}

				ImGui::SliderFloat("Metallic Value", &component.metallicValue, 0.f, 1.f);



				ImGui::Separator();
				ImGui::Text("roughness Texture");

				if (component.roughness) ImGui::Image((void*)component.roughness->GetRendererID(), ImVec2(150, 150));
				ImGui::SameLine();
				ImGui::Checkbox("Use roughness", &component.roughnessTextureUse);

				if (ImGui::Button("Open roughness"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						component.roughness = Texture2D::Create(filepath.value());
					}
				}

				ImGui::SliderFloat("Roughness Value", &component.roughnessValue, 0.f, 1.f);




				ImGui::Separator();
				ImGui::Text("ao Texture");

				if (component.ao) ImGui::Image((void*)component.ao->GetRendererID(), ImVec2(150, 150));
				ImGui::SameLine();

				if (ImGui::Button("Open ao"))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						component.ao = Texture2D::Create(filepath.value());
					}
				}
	
			});

	}

}
