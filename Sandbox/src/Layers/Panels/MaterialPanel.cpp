#include "MaterialPanel.h"

#include "CrashEngine/Core/Log.h"
#include "CrashEngine/Utils/PlatformUtils.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>



namespace CrashEngine {
	MaterialPanel::MaterialPanel()
	{
		texcreator.reset(new TextureCreator());
		albedo_loaded = texcreator->CreateTexture(200, 200, glm::vec3(0.2,0.2,0.2));
		metallic_loaded = texcreator->CreateTexture(200, 200, glm::vec3(0.2, 0.2, 0.2));
		roughness_loaded = texcreator->CreateTexture(200, 200, glm::vec3(0.2, 0.2, 0.2));
	}

	void MaterialPanel::SetEntity(Mesh* mesh)
	{
		m_Mesh = mesh;

		if(m_Mesh->material->albedaLoad)albedo_loaded = m_Mesh->material->albedo;
		else albedo_loaded = texcreator->CreateTexture(200, 200, glm::vec3(0.2, 0.2, 0.2));
		
		if (m_Mesh->material->metallicLoad)metallic_loaded = m_Mesh->material->metallic;
		else metallic_loaded = texcreator->CreateTexture(200, 200, glm::vec3(0.2, 0.2, 0.2));

		if (m_Mesh->material->roughnessLoad)roughness_loaded = m_Mesh->material->roughness;
		else roughness_loaded = texcreator->CreateTexture(200, 200, glm::vec3(0.2, 0.2, 0.2));


		lastalbedoColor = m_Mesh->material->lastalbedoColor;
		lastmetallicValue = m_Mesh->material->metallicValue;
		lastroughnessValue = m_Mesh->material->roughnessValue;
	}

	void MaterialPanel::OnImGuiRender()
	{
		ImGui::Begin("Material");


		if (m_Mesh->material)
		{
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			bool material = ImGui::TreeNodeEx(m_Mesh->material->name.c_str(), treeNodeFlags);

			if (material)
			{
				ImGui::NewLine();
				ImGui::Separator();
				ImGui::Text("Albedo Texture");

				if (ImGui::ImageButton((void*)albedo_loaded->GetRendererID(), ImVec2(200, 200)))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						albedo_loaded = Texture2D::Create(filepath.value());
						m_Mesh->material->albedo = albedo_loaded;
						m_Mesh->material->albedaLoad = true;
					}
				}

				ImGui::SameLine();
				ImGui::Checkbox("Use albedo", &m_Mesh->material->albedoTextureUse);


				if (!m_Mesh->material->albedoTextureUse)
				{
					ImGui::ColorPicker3("Albedo color", &m_Mesh->material->albedoColor.x);

					if(lastalbedoColor!= m_Mesh->material->albedoColor)m_Mesh->material->albedo = m_Mesh->material->texcreator->CreateTexture(200, 200, m_Mesh->material->albedoColor);
				}
				else
				{
					if(albedo_loaded)m_Mesh->material->albedo = albedo_loaded;
				}


				ImGui::NewLine();
				ImGui::Separator();
				ImGui::Text("Metallic Texture");

				if (ImGui::ImageButton((void*)metallic_loaded->GetRendererID(), ImVec2(200, 200)))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						metallic_loaded = Texture2D::Create(filepath.value());
						m_Mesh->material->metallic = metallic_loaded;
						m_Mesh->material->metallicLoad = true;
					}

				}

				ImGui::SameLine();
				ImGui::Checkbox("Use metallic", &m_Mesh->material->metallicTextureUse);


				if (!m_Mesh->material->metallicTextureUse)
				{
					ImGui::SliderFloat("Metallic Value", &m_Mesh->material->metallicValue, 0.f, 1.f);
					if (lastmetallicValue != m_Mesh->material->metallicValue)m_Mesh->material->metallic = m_Mesh->material->texcreator->CreateTexture(200, 200, glm::vec3(m_Mesh->material->metallicValue, m_Mesh->material->metallicValue, m_Mesh->material->metallicValue));
				}
				else
				{
					if (metallic_loaded)m_Mesh->material->metallic = metallic_loaded;
				}


				ImGui::NewLine();
				ImGui::Separator();
				ImGui::Text("Roughness Texture");

				if (ImGui::ImageButton((void*)roughness_loaded->GetRendererID(), ImVec2(200, 200)))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						roughness_loaded = Texture2D::Create(filepath.value());
						m_Mesh->material->roughness = roughness_loaded;
						m_Mesh->material->roughnessLoad = true;
					}

				}

				ImGui::SameLine();
				ImGui::Checkbox("Use Roughness", &m_Mesh->material->roughnessTextureUse);


				if (!m_Mesh->material->roughnessTextureUse)
				{
					ImGui::SliderFloat("Roughness Value", &m_Mesh->material->roughnessValue, 0.f, 1.f);
					if (lastroughnessValue != m_Mesh->material->roughnessValue)m_Mesh->material->roughness = m_Mesh->material->texcreator->CreateTexture(200, 200, glm::vec3(m_Mesh->material->roughnessValue, m_Mesh->material->roughnessValue, m_Mesh->material->roughnessValue));
				}
				else
				{
					if (roughness_loaded)m_Mesh->material->roughness = roughness_loaded;
				}


				ImGui::NewLine();
				ImGui::Separator();
				ImGui::Text("Normal Texture");

				if (ImGui::ImageButton((void*)m_Mesh->material->normal->GetRendererID(), ImVec2(200, 200)))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						m_Mesh->material->normal = Texture2D::Create(filepath.value());
					}

				}

				ImGui::NewLine();
				ImGui::Separator();
				ImGui::Text("Ambient Occlusion Texture");

				if (ImGui::ImageButton((void*)m_Mesh->material->ao->GetRendererID(), ImVec2(200, 200)))
				{
					std::optional<std::string> filepath = FileDialogs::OpenFile("");
					if (filepath)
					{
						m_Mesh->material->ao = Texture2D::Create(filepath.value());
					}

				}



				

				ImGui::TreePop();
			}


		}

		ImGui::End();
	}
}