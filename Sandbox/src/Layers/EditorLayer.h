#pragma once

#include <Crash.h>

#include "imgui/imgui.h"

#include "CrashEngine/Renderer/Renderer.h"

#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"




namespace CrashEngine {

	class Editor : public CrashEngine::Layer
	{
	public:
		Editor();

		void OnUpdate() override;

		virtual void OnImGuiRender() override;

		void OnEvent(CrashEngine::Event& event) override;

	private:
		std::shared_ptr<ImGuiLayer> imguilayer;

		//ImVec2 OldWindowSize = ImVec2(0,0);

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<UniformBuffer> m_MatrixUB;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Model> backpack;

		Shader* basicShader;

		Shader* pbrShader;
		Shader* pbrTextureShader;
		Shader* equirectangularToCubemapShader;
		Shader* irradianceShader;
		Shader* prefilterShader;
		Shader* brdfShader;
		Shader* backgroundShader;

		glm::mat4 model;
		//glm::mat4 view;
		//glm::mat4 projection;

		double lastTime = 0;

		// lights
		glm::vec3 lightPositions[4] = {
			glm::vec3(-10.0f,  10.0f, 10.0f),
			glm::vec3(10.0f,  10.0f, 10.0f),
			glm::vec3(-10.0f, -10.0f, 10.0f),
			glm::vec3(10.0f, -10.0f, 10.0f),
		};
		glm::vec3 lightColors[4] = {
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(300.0f, 300.0f, 300.0f)
		};
		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;

		std::shared_ptr<Sphere> sphere;
		std::shared_ptr<Square> square;
		std::shared_ptr<Quad> quad;


		Model* testModel;

		std::shared_ptr<Texture2D> albedo;
		std::shared_ptr<Texture2D> normal;
		std::shared_ptr<Texture2D> metallic;
		std::shared_ptr<Texture2D> roughness;
		std::shared_ptr<Texture2D> ao;



		std::shared_ptr<TextureHDR> HDR;
		std::shared_ptr<CubemapTexture> Cubemap;
		std::shared_ptr<CubemapTexture> Irradiancemap;
		std::shared_ptr<CubemapTexture> Prefiltermap;
		std::shared_ptr<Texture2D> brdfLUTTexture;


		std::shared_ptr<Framebuffer> renderFramebuffer;

		std::shared_ptr<Framebuffer> Framebuffer;
		std::shared_ptr<Renderbuffer> Renderbuffer;
	};

}