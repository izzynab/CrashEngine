#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "CrashEngine/Renderer/Framebuffer.h"
#include "CrashEngine/Renderer/Texture.h"
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Debug/LinesDrawer.h"

namespace CrashEngine {

	class DirectionalLight
	{
	public:
		DirectionalLight();

		void DrawCSM(Camera *camera, Shader* defferedShader);

	public:
		glm::mat4 lightView;
		glm::mat4 lightOrthoProj;

	public:
		glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 color = glm::vec3(1,1,1);
		float intensity = 50.f;

		Shader* depthMapShader;


		std::vector<std::shared_ptr<Texture2D>> depthMap;
		std::shared_ptr<Framebuffer> depthFramebuffer;

		float near_plane = -10.f, far_plane = 17.5f;
		float size = 10.f;
	private:
		std::vector<float> m_cascadeEnd;	
		std::vector<glm::mat4> mCascadeWorldViewProj;

	};
}