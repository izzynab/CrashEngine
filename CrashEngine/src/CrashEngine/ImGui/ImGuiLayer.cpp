#include "cepch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "CrashEngine/Core/Application.h"

#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "CrashEngine/Scene/SceneSerializer.h"
//#include "CrashEngine/Renderer/RenderProperties.h"

#include "ImGuizmo.h"
#include "ImGuiFileDialog.h"
#include "IconsFontAwesome5.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace CrashEngine {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.Fonts->AddFontDefault();
		io.Fonts->AddFontFromFileTTF("../fonts/arial.ttf", 16.0f);

		io.IniFilename = "imgui.ini";
		io.IniSavingRate = 1;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		//setup ImGuiFileDialogIcons
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF("../fonts/" FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);

		ImGuiFileDialog::Instance()->SetExtentionInfos(".cpp", ImVec4(1, 1, 1, 0.9), ICON_FA_FILE);
		ImGuiFileDialog::Instance()->SetExtentionInfos(".h", ImVec4(1, 1, 1, 0.9), ICON_FA_FILE);
		ImGuiFileDialog::Instance()->SetExtentionInfos(".hpp", ImVec4(1, 1, 1, 0.9), ICON_FA_FILE);

		ImGuiFileDialog::Instance()->SetExtentionInfos(".obj", ImVec4(1, 1, 1, 0.9), ICON_FA_CUBE);
		ImGuiFileDialog::Instance()->SetExtentionInfos(".fbx", ImVec4(1, 1, 1, 0.9), ICON_FA_CUBE);
		
		ImGuiFileDialog::Instance()->SetExtentionInfos(".crash", ImVec4(1, 1, 1, 0.9), ICON_FA_ARCHWAY);

		ImGuiFileDialog::Instance()->SetExtentionInfos(".jpg", ImVec4(1, 1, 1, 0.9), ICON_FA_FILE_IMAGE);
		ImGuiFileDialog::Instance()->SetExtentionInfos(".png", ImVec4(1, 1, 1, 0.9), ICON_FA_FILE_IMAGE);
		ImGuiFileDialog::Instance()->SetExtentionInfos(".tga", ImVec4(1, 1, 1, 0.9), ICON_FA_FILE_IMAGE);

        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

	}

	void ImGuiLayer::Dockspace(std::shared_ptr<RenderProperties>& renderProperties, std::vector<std::shared_ptr<Framebuffer>> framebuffers)
	{
		bool s = true;
		bool* show = &s;

		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", show, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
		{
			ImGui::PopStyleVar(2);
		}	

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		
	}

	void ImGuiLayer::MainMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{

			if (ImGui::BeginMenu("Tools"))
			{
				ImGui::MenuItem("Style editor", NULL, &EditorStyleEnabled);
				ImGui::MenuItem("Menu", NULL, &MenuEnabled);
				ImGui::MenuItem("Window metrices", NULL, &WindowMetricsEnabled);

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

	}

	void ImGuiLayer::StyleEditor()
	{
		ImGui::Begin("Style Editor");
		ImGui::ShowStyleEditor();
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::End();
	}

	void ImGuiLayer::Menu()
	{
        //ImGui::Begin("Menu");
		ImGui::ShowDemoWindow();
        //ImGui::End();
		
	}

	void ImGuiLayer::WindowMetrics()
	{
		ImGui::ShowMetricsWindow();
		
	}

	void ImGuiLayer::OnImGuiRender()
	{
		//ImGui::Text(ICON_FA_PAINT_BRUSH "  Paint");

		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Content Browser", ".h,.cpp,.crash,.obj,.png","../Content", "");

		// display
		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			// action if OK
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action
				CE_INFO("{0}  {1}", filePathName, filePath);
				//todo: make content browser
			}

			// close
			ImGuiFileDialog::Instance()->Close();
		}
		

	}

}