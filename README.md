# CrashEngine
 
Hello.
This is my personal game engine Project.I have been working on this project since august 2020.
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
 Use `git clone --recursive https://github.com/izzynab/CrashEngine` to clone this repository.If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.
 Generate projects and solution using GenerateProjects.bat file. 
 When rebuild you probably must copy assimp.lib from folder "CrashEngine\vendor\assimp\bin\Debug-windows-x86_64\Assimp\assimpllibhere" to "CrashEngine\vendor\assimp\bin\Debug-windows-x86_64\Assimp"
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
  - Content browser   
  - optimized assets loader(each asset should be loaded to engine once and other things)
  - improve loading times(khronos forum site)

  - good hierarchy/components system

  - Instanced rendering
  - CSM/PCSS
  - Atmospheric Scattering 
  - Point lights with deffered rendering
  - god rays
 
  - mouse picking objects from scene
  - terrain generator
 
 #### Long term goals
  - C# scripts
  - Runtime
  - Animation
  - Particle System
  - Physics engine
  - Audio engine
  - Port to DX12

![image](https://user-images.githubusercontent.com/47477184/120243346-a3c5c780-c267-11eb-91ea-eb3ce4b1e077.png)
![image](https://user-images.githubusercontent.com/47477184/120243366-ac1e0280-c267-11eb-9e08-d148cdc44ee0.png)
![image](https://user-images.githubusercontent.com/47477184/120243460-edaead80-c267-11eb-93e2-5beda3c3f02d.png)

