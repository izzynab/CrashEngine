include "./vendor/premake/premake_customization/solution_items.lua"

workspace "CrashEngine"
	architecture "x64"

	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"]		= "%{wks.location}/CrashEngine/vendor/GLFW/include"
IncludeDir["GLAD"]		= "%{wks.location}/CrashEngine/vendor/GLAD/include"
IncludeDir["ImGui"]		= "%{wks.location}/CrashEngine/vendor/imgui"
IncludeDir["glm"]		= "%{wks.location}/CrashEngine/vendor/glm"
IncludeDir["stb"]		= "%{wks.location}/CrashEngine/vendor/stb_image"
IncludeDir["assimp"]	= "%{wks.location}/CrashEngine/vendor/assimp/include"
IncludeDir["entt"]		= "%{wks.location}/CrashEngine/vendor/entt/include"



group "Dependencies"
	include "CrashEngine/vendor/GLFW"
	include "CrashEngine/vendor/GLAD"
	include "CrashEngine/vendor/imgui"
	include "CrashEngine/vendor/assimp"
group ""


include "CrashEngine"
include "Sandbox"
