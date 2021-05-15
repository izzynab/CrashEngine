#include "cepch.h"
#include "RenderProperties.h"

namespace CrashEngine {
	RenderProperties::RenderProperties()
	{
		quad.reset(new Quad());

		Application::Get().GetDebugger().Begin();

		//------------framebuffers--------------------
		FramebufferSpecification spec;
		spec.Height = Application::Get().GetWindow().GetHeight();
		spec.Width = Application::Get().GetWindow().GetWidth();

		deferredframebuffer = Framebuffer::Create(spec, false);
		deferredframebuffer->Bind();
		deferredframebuffer->CreateTexture(0, Color::RGBA);//position
		deferredframebuffer->CreateTexture(1, Color::RGBA);//albedo
		deferredframebuffer->CreateTexture(2, Color::RGBA);//normal
		deferredframebuffer->CreateTexture(3, Color::RGBA);//metallic roughness ao
		deferredframebuffer->InitializeMultipleTextures(4);

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

		forwardShader.reset(Shader::Create("pbr.vert", "pbr.frag"));
		forwardShader->Bind();

		forwardShader->SetUniformInt("albedoMap", 0);
		forwardShader->SetUniformInt("normalMap", 1);
		forwardShader->SetUniformInt("metallicMap", 2);
		forwardShader->SetUniformInt("roughnessMap", 3);
		forwardShader->SetUniformInt("aoMap", 4);

		forwardShader->SetUniformInt("irradianceMap", 5);
		forwardShader->SetUniformInt("prefilterMap", 6);
		forwardShader->SetUniformInt("brdfLUT", 7);
		forwardShader->SetUniformInt("shadowMap", 8);

		//---------------scene-----------------------
		m_ActiveScene = std::make_shared<Scene>();

		//-------------Uniform Buffer-----------------
		UniformBufferLayout uniformLayout = {
			{ ShaderDataType::Mat4, "projection" },
			{ ShaderDataType::Mat4, "view"},
		};

		m_MatrixUB.reset(UniformBuffer::Create(uniformLayout, 0));
		m_MatrixUB->linkShader(forwardShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(deferredShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(m_ActiveScene->skyLight->GetSkyShader()->GetID(), "Matrices");
		m_MatrixUB->linkShader(GBufferShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(m_ActiveScene->ssao->ssaoShader->GetID(), "Matrices");
		m_MatrixUB->linkShader(Application::Get().GetDebugger().GetLinesShader().GetID(), "Matrices");
		m_MatrixUB->linkShader(Application::Get().GetDebugger().GetPointsShader().GetID(), "Matrices");

		//glm::mat4 projection = m_ActiveScene->GetActiveCamera().GetProjectionMatrix();
		//m_MatrixUB->setData("projection", glm::value_ptr(projection));

	}
}
