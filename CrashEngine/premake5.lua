project "CrashEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

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
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
	}

	links 
	{ 
		"GLAD",
		"GLFW",
		"ImGui",
		"yaml-cpp",
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