workspace "CrashEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "CrashEngine/vendor/GLFW/include"
IncludeDir["GLAD"] = "CrashEngine/vendor/GLAD/include"
IncludeDir["ImGui"] = "CrashEngine/vendor/imgui"
IncludeDir["glm"] = "CrashEngine/vendor/glm"
IncludeDir["stb"] = "CrashEngine/vendor/stb_image"
IncludeDir["assimp"] = "CrashEngine/vendor/assimp/include"
IncludeDir["Freetype"] = "CrashEngine/vendor/freetype-lib/include"

include "CrashEngine/vendor/GLFW"
include "CrashEngine/vendor/GLAD"
include "CrashEngine/vendor/imgui"
include "CrashEngine/vendor/assimp"
include "CrashEngine/vendor/freetype-lib"

project "CrashEngine"
	location "CrashEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cepch.h"
	pchsource "CrashEngine/src/cepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/assimp/include/assimp/**.hpp",
		"%{prj.name}/vendor/assimp/include/assimp/**.inl",
		"%{prj.name}/vendor/assimp/include/assimp/**.h",
		"%{prj.name}/vendor/assimp/include/assimp/**.in",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Freetype}",
	}

	links 
	{ 
		"GLAD",
		"GLFW",
		"ImGui",
		"Assimp",
		"Freetype",
		"opengl32.lib",
	}
	

	filter "system:windows"
		systemversion "10.0.18362.0"

		defines
		{
			"CE_PLATFORM_WINDOWS",
			"CE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "CE_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "CE_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "CE_DIST"
		buildoptions "/MD"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"CrashEngine/vendor/spdlog/include",
		"CrashEngine/src",
		"CrashEngine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Freetype}",
	}

	links
	{
		"CrashEngine"
	}

	filter "system:windows"
		systemversion "10.0.18362.0"

		defines
		{

			"CE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CE_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "CE_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "CE_DIST"
		buildoptions "/MD"
		optimize "on" 
