#include "cepch.h"
#include "RenderProperties.h"
#include "CrashEngine/Core/Application.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace CrashEngine {
	RenderProperties::RenderProperties()
	{
		quad.reset(new Quad());

		Application::Get().GetDebugger().Begin();

		//------------shaders-------------------------
		deferredShader.reset(Shader::Create("basic.vert", "deferred.frag"));
		deferredShader->Bind();

		deferredShader->SetUniformInt("position", 0);
		deferredShader->SetUniformInt("albedo", 1);
		deferredShader->SetUniformInt("normal", 2);
		deferredShader->SetUniformInt("MetalRoughAO", 3);

		deferredShader->SetUniformInt("irradianceMap", 4);
		deferredShader->SetUniformInt("prefilterMap", 5);
		deferredShader->SetUniformInt("brdfLUT", 6);
		deferredShader->SetUniformInt("ssao", 7);
		deferredShader->SetUniformInt("shadowMap[0]", 8);
		deferredShader->SetUniformInt("shadowMap[1]", 9);
		deferredShader->SetUniformInt("shadowMap[2]", 10);

		GBufferShader.reset(Shader::Create("pbr.vert", "gbuffer.frag"));
		GBufferShader->Bind();
		GBufferShader->SetUniformInt("albedoMap", 0);
		GBufferShader->SetUniformInt("normalMap", 1);
		GBufferShader->SetUniformInt("metallicMap", 2);
		GBufferShader->SetUniformInt("roughnessMap", 3);
		GBufferShader->SetUniformInt("aoMap", 4);

		//---------------scene-----------------------
		m_ActiveScene = std::make_shared<Scene>();

		//-------------Uniform Buffer-----------------
		UniformBufferLayout uniformLayout = {
			{ ShaderDataType::Mat4, "projection" },
			{ ShaderDataType::Mat4, "view"},
		};

		m_MatrixUB.reset(UniformBuffer::Create(uniformLayout, 0));
		m_MatrixUB->linkShader(deferredShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(GBufferShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(m_ActiveScene->ssao->GetShaderID(), "Matrices");
		m_MatrixUB->linkShader(m_ActiveScene->skyLight->GetSkyShader()->GetID(), "Matrices");
		m_MatrixUB->linkShader(Application::Get().GetDebugger().GetLinesShader().GetID(), "Matrices");
		m_MatrixUB->linkShader(Application::Get().GetDebugger().GetPointsShader().GetID(), "Matrices");

		//glm::mat4 projection = m_ActiveScene->GetActiveCamera().GetProjectionMatrix();
		//m_MatrixUB->setData("projection", glm::value_ptr(projection));

	}

	void RenderProperties::AddView(float width, float height, std::string name)
	{
		allViews++;
		view view;
		view.name = name;
		view.id = allViews;

		FramebufferSpecification spec;
		spec.Height = height;
		spec.Width = width;

		std::shared_ptr<Framebuffer> deferredframebuffer;
		deferredframebuffer = Framebuffer::Create(spec, false);
		deferredframebuffer->Bind();
		deferredframebuffer->CreateTexture(0, Color::RGBA);//position
		deferredframebuffer->CreateTexture(1, Color::RGBA);//albedo
		deferredframebuffer->CreateTexture(2, Color::RGBA);//normal
		deferredframebuffer->CreateTexture(3, Color::RGBA);//metallic roughness ao
		deferredframebuffer->InitializeMultipleTextures(4);

		view.deferredframebuffer = deferredframebuffer;

		glm::vec3 position = glm::vec3(1.0f,10.0f,1.0f);
		std::shared_ptr<Camera> camera;
		camera.reset(new Camera(position, width, height));

		std::shared_ptr<CameraController> cameraController;
		cameraController.reset(new CameraController(camera));
		view.cameraController = cameraController;
		view.camera = camera;

		views.push_back(view);

		m_ActiveScene->postProcess->AddView(width, height, allViews);
		m_ActiveScene->ssao->AddView(width, height, allViews);
	}
}
