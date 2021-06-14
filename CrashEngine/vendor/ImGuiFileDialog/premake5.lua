project "ImGuiFileDialog"
    kind "StaticLib"
    language "C++"
     staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "ImGuiFileDialog.cpp",
        "ImGuiFileDialog.h",
        "ImGuiFileDialogConfig.h",
        "dirent/dirent.h"
    }

    includedirs
	{	
		"../imgui",
        "../imgui"
	}

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"