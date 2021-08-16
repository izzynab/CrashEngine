#include "cepch.h"
#include "DirectionalLight.h"

#include "CrashEngine/Renderer/RenderCommand.h"
#include "CrashEngine/Core/Defines.h"

#include "CrashEngine/Scene/Scene.h"
#include "CrashEngine/Core/Application.h"


namespace CrashEngine
{
	DirectionalLight::DirectionalLight()
	{
		depthMapShader = Shader::Create("depthMap.vert", "depthMap.frag");
		blurShader = Shader::Create("Basic.vert", "blur.frag");

		FramebufferSpecification depthspec;
		depthspec.Height = 1024*4;
		depthspec.Width = 1024*4;

		depthFramebuffer = Framebuffer::Create(depthspec, true);

		depthMap.push_back(Texture2D::Create(1024 * 4, 1024 * 4));
		depthMap.push_back(Texture2D::Create(1024 * 3, 1024 * 3));
		depthMap.push_back(Texture2D::Create(1024 * 2, 1024 * 2));
		depthMap.push_back(Texture2D::Create(1024, 1024));


		m_cascadeEnd.push_back(1.0f);
		m_cascadeEnd.push_back(30.0f);
		m_cascadeEnd.push_back(70.f);
		m_cascadeEnd.push_back(120.f);
		m_cascadeEnd.push_back(180.f);

		mCascadeWorldViewProj.push_back(glm::mat4(1));
		mCascadeWorldViewProj.push_back(glm::mat4(1));
		mCascadeWorldViewProj.push_back(glm::mat4(1));
		mCascadeWorldViewProj.push_back(glm::mat4(1));

	}


	void DirectionalLight::DrawCSM(Camera* camera,Shader*defferedShader, std::shared_ptr<Scene>& m_ActiveScene)
	{
		//normalized rotation vector(Spherical coordinate)
		glm::vec3 rot = glm::vec3(
			glm::cos(rotation.x) * glm::sin(rotation.y),
			glm::sin(rotation.x) * glm::sin(rotation.y),
			glm::cos(rotation.y));

		glm::vec3 up = glm::vec3(0.0f, 1.f, 0.0f);

		float fov = camera->fov;
		float AspectRatio = camera->ScreenWidth / camera->ScreenHeight;
	
		for (int i = 0; i < 4; i++)
		{
			//splitted frustum
			glm::mat4 cameraProjectionMatrix = glm::perspective(glm::radians(fov), AspectRatio, m_cascadeEnd[i], m_cascadeEnd[i+1]);
			glm::mat4 cameraProjView = cameraProjectionMatrix * camera->GetViewMatrix();

			std::array<glm::vec3, 8> frustumCorners = CalculateFrustumCoreners(cameraProjView);

			//calculate frustum center and distance from near to far plane of the frustum
			float maxZ = -std::numeric_limits<float>::max();
			float minZ = std::numeric_limits<float>::max();
			glm::vec3 frustumCenter = glm::vec3(0);
			for (int i = 0; i < 8; i++)
			{
				frustumCenter += frustumCorners[i];
				minZ = glm::min(minZ, frustumCorners[i].z);
				maxZ = glm::max(maxZ, frustumCorners[i].z);
			}
			frustumCenter /= 8.f;

			glm::vec3 lightDir = rot;
			glm::vec3 lightPos = frustumCenter + lightDir * (m_cascadeEnd[i+1] - m_cascadeEnd[i]);

			glm::mat4 mLightView = glm::lookAt(lightPos, frustumCenter, glm::vec3(0.0f, 1.0f, 0.0f));

			for (int i = 0; i < 8; ++i)
			{
				frustumCorners[i] = glm::vec3(mLightView * glm::vec4(frustumCorners[i], 1.0f));
			}

			float minX = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::min();
			float minY = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::min();
			minZ = std::numeric_limits<float>::max();
			maxZ = std::numeric_limits<float>::min();

			for (int i = 0; i < 8; ++i)
			{
				minX = std::min(minX, frustumCorners[i].x);
				maxX = std::max(maxX, frustumCorners[i].x);
				minY = std::min(minY, frustumCorners[i].y);
				maxY = std::max(maxY, frustumCorners[i].y);
				minZ = std::min(minZ, frustumCorners[i].z);
				maxZ = std::max(maxZ, frustumCorners[i].z);
			}

			glm::mat4 mLightProj = glm::ortho(minX, maxX, minY, maxY, m_cascadeEnd[0], maxZ- minZ);

			mCascadeWorldViewProj[i] = mLightProj * mLightView;

		}


		for (int i = 0; i < 4; i++)
		{
			glm::mat4 lightSpaceMatrix =  mCascadeWorldViewProj[i];

			defferedShader->Bind();
			defferedShader->SetUniformMat4("lightSpaceMatrix[" + std::to_string(i) + "]", lightSpaceMatrix);
			defferedShader->SetUniformFloat("cascadeEndClipSpace[" + std::to_string(i) + "]", m_cascadeEnd[i + 1]);
			
			depthMapShader->Bind();
			depthMapShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

			RenderCommand::SetViewport(1024 *(4-i), 1024 * (4 - i));
			depthFramebuffer->Bind();
			depthFramebuffer->SetTexture(CE_TEXTURE_2D, depthMap[i]->GetRendererID(),0);
			RenderCommand::SetClearColor(glm::vec4(0, 0.5, 0, 1));
			RenderCommand::Clear();

			m_ActiveScene->DepthRender(depthMapShader);

		}

		depthFramebuffer->Unbind();
	}

	std::array<glm::vec3, 8> DirectionalLight::CalculateFrustumCoreners(glm::mat4 projViewMatrix)
	{

		glm::mat4 inv = glm::inverse(projViewMatrix);

		glm::vec4 f[8u] =
		{
			// near face
			{1, 1, -1, 1.f},
			{-1, 1, -1, 1.f},
			{1, -1, -1, 1.f},
			{-1, -1, -1, 1.f},

			// far face
			{1, 1, 1, 1.f},
			{-1, 1, 1 , 1.f},
			{1, -1, 1 , 1.f},
			{-1, -1,1, 1.f},
		};

		std::array<glm::vec3, 8> frustumCorners;
		for (int i = 0; i < 8; i++)
		{
			glm::vec4 ff = inv * f[i];
			frustumCorners[i].x = ff.x / ff.w;
			frustumCorners[i].y = ff.y / ff.w;
			frustumCorners[i].z = ff.z / ff.w;
		}

		return frustumCorners;
	}

	glm::vec3 DirectionalLight::CalculateFrustumCenter(glm::mat4 projViewMatrix)
	{
		std::array<glm::vec3, 8> frustumCorners = CalculateFrustumCoreners(projViewMatrix);
		glm::vec3 frustumCenter = glm::vec3(0);

		for (int i = 0; i < 8; i++)
		{
			frustumCenter += frustumCorners[i];
		} 
		frustumCenter /= 8.f;

		return frustumCenter;
	}

}