#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "CrashEngine/Renderer/Framebuffer.h"
#include "CrashEngine/Renderer/Texture.h"
#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Scene/Scene.h"

namespace CrashEngine {

	class DirectionalLight
	{
	public:
		DirectionalLight();

		void DrawCSM();

	private:
		glm::vec3 CalcSphereCenter(float start, float end);
		float CalcSphereRadius(glm::vec3 sphereCenter, float Far);

	public:
		glm::mat4 lightView;
		glm::mat4 lightOrthoProj;

	public:
		glm::vec3 rotation = glm::vec3(1.f, 1.f, 0.f);
		glm::vec3 position = glm::vec3(-2.0f, 4.0f, -1.0f);
		glm::vec3 color = glm::vec3(1,1,1);
		float intensity = 50.f;

		Shader* depthMapShader;
		Shader* pbrTextureShader;
		Camera* camera;
		std::shared_ptr<Scene> m_ActiveScene;
		float Height;
		float Width;

		std::vector<std::shared_ptr<DepthTexture>> depthMap;
	private:
		std::shared_ptr<Framebuffer> depthFramebuffer;


		std::vector<float> m_cascadeEnd;
		std::vector<float> mCascadeRadius;
		std::vector<glm::vec3> mCascadeCenter;
		std::vector<glm::mat4> mCascadeWorldViewProj;

		float mCascadeRes = 1024;
	};
}