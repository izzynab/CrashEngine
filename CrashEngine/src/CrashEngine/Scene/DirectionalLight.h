#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "CrashEngine/Renderer/Framebuffer.h"
#include "CrashEngine/Renderer/Texture.h"
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Debug/LinesDrawer.h"

namespace CrashEngine {

	class Scene;

	class DirectionalLight
	{
	public:
		DirectionalLight();

		void DrawCSM(Camera *camera, Shader* defferedShader, std::shared_ptr<Scene>& m_ActiveScene);

	private:
		std::array<glm::vec3,8> CalculateFrustumCoreners(glm::mat4 projViewMatrix);
		glm::vec3 CalculateFrustumCenter(glm::mat4 projViewMatrix);
	public:
		glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 color = glm::vec3(1,1,1);
		float intensity = 50.f;

		std::vector<std::shared_ptr<Texture2D>> depthMap;
		std::shared_ptr<Framebuffer> depthFramebuffer;

	private:
		std::vector<float> m_cascadeEnd;	
		std::vector<glm::mat4> mCascadeWorldViewProj;

		Shader* depthMapShader;
		Shader* blurShader;
	};
}