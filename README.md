# CrashEngine
 
Hello.
This is my personal game engine Project.I have been working on this project since 2019.
Crash Engine is written in C++ using OpenGL libraries.
Engine is based on on the first episodes of The Cherno's Hazel Engine series.
Crash engine is using libraries:
 - [assimp](https://github.com/assimp/assimp)
 - [entt](https://github.com/skypjack/entt)
 - [GLAD](https://github.com/Dav1dde/glad)
 - [GLFW](https://github.com/glfw/glfw)
 - [glm](https://github.com/g-truc/glm)
 - [imgui](https://github.com/ocornut/imgui)
 - [imguizmo](https://github.com/CedricGuillemet/ImGuizmo)
 - [spdlog](https://github.com/gabime/spdlog)
 - [stb_image](https://github.com/nothings/stb)
 - [yaml](https://github.com/jbeder/yaml-cpp)

 ## Getting started
 You can clone repository and generate project solution using GenerateProjects.bat file. You also need to copy from "CrashEngine\vendor\assimp\bin\Debug-windows-x86_64\Assimp\folder with assimp.lib" file assimp.lib to "CrashEngine\vendor\assimp\bin\Debug-windows-x86_64\Assimp\". Maybe i will fix it.
 #### Features of Crash Engine
  - pbr pipeline
  - cubemaps
  - deffered rendering
  - FXAA
  - Bloom 
  - Directional Light
  - SSAO
  - Manipulation Gizmo
  - Scene Serialization

 #### To do 
  - CSM/PCSS
  - Atmospheric Scattering 
  - Point lights with deffered rendering
  - god rays
  - mouse picking objects from scene
  - terrain generator
 
 #### Long term goals
  - C# scripts and improve Entity system
  - Animation
  - Particle System
  - Physics engine
  - Audio engine
  - Game exe app
  - Port to DX12
