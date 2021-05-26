#pragma once

//#include "CrashEngine/Core/Application.h"
#include "CrashEngine/Scene/Scene.h"

#include "Shader.h"
#include "BasicShapes.h"
#include "Framebuffer.h"


namespace CrashEngine {

	class RenderProperties
	{
		struct view
		{
			std::string name;
			uint32_t id;
			bool isViewActive = false;
			bool isOpen = true;

			std::shared_ptr<Framebuffer> deferredframebuffer;
			std::shared_ptr<Camera> camera;
			std::shared_ptr<CameraController> cameraController;
		};

	public:
		RenderProperties();

		void AddView(float width, float height, std::string name);

	private:
		/*Currently active scene*/
		std::shared_ptr<Scene> m_ActiveScene;

		/*Uniform Buffer stores view and projection matrix*/
		std::shared_ptr<UniformBuffer> m_MatrixUB;

		/*GBuffer shader gives buffers to deffered render*/
		std::shared_ptr<Shader> GBufferShader;

		/*Shader draws pbr scene from gbuffer images*/
		std::shared_ptr<Shader> deferredShader;

		std::shared_ptr<Quad> quad;

		/*Framebuffers which stores GBuffer images*/
		//std::vector<std::shared_ptr<Framebuffer>> deferredframebuffers;

		/*Cameras for views*/
		//std::vector<std::shared_ptr<Camera>> cameras;

	public:
		inline std::shared_ptr<Scene> GetScene() { return m_ActiveScene; }
		inline std::shared_ptr<UniformBuffer> GetUBO() { return m_MatrixUB; }
		inline std::shared_ptr<Framebuffer> GetDefferedFramebuffer(int id) { return views[id].deferredframebuffer; }
		inline std::shared_ptr<Camera> GetCamera(int id) { return views[id].camera; }
		inline std::shared_ptr<CameraController> GetCameraController(int id) { return views[id].cameraController; }

		inline std::shared_ptr<Shader> GetDefferedShader() { return deferredShader; }
		inline std::shared_ptr<Shader> GetGBufferShader() { return GBufferShader; }

		inline std::shared_ptr<Quad> GetQuad() { return quad; }

		inline int GetViewsNumber() { return views.size(); }
		inline std::string GetViewName(uint32_t id) { return views[id].name; }
		inline bool IsViewActive(uint32_t id) { return views[id].isViewActive; }
		inline void SetViewActive(uint32_t id, bool isActive) { views[id].isViewActive = isActive; }

	public:
		std::vector<view> views;

	private:
		uint32_t allViews = 0;

	};
}