project "assimp"
  kind "StaticLib"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
      "include/assimp/**.h",    
      "include/assimp/**.hpp",
      "include/assimp/**.inl",
      "include/assimp/**.in",
    }

    includedirs {
      "include",
      "contrib/irrXML",
      "contrib/zlib",
      "contrib/rapidjson/include",
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
