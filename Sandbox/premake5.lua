project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

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
		"%{IncludeDir.entt}",

		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Freetype}",
		"%{IncludeDir.yaml_cpp}",
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
