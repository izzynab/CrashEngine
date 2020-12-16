project "CrashEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cepch.h"
	pchsource "src/cepch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/assimp/include/assimp/**.hpp",
		"vendor/assimp/include/assimp/**.inl",
		"vendor/assimp/include/assimp/**.h",
		"vendor/assimp/include/assimp/**.in",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
	}

	links 
	{ 
		"GLAD",
		"GLFW",
		"ImGui",
		"assimp",
		"yaml-cpp",
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
		symbols "on"

	filter "configurations:Release"
		defines "CE_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "CE_DIST"
		optimize "on"