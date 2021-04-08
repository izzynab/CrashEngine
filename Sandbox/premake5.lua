project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/CrashEngine/vendor/spdlog/include",
		"%{wks.location}/CrashEngine/src",
		"%{wks.location}/CrashEngine/vendor",

		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",

		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.ImGuizmo}",


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
		symbols "on"

	filter "configurations:Release"
		defines "CE_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "CE_DIST"
		optimize "on" 
