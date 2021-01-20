#include "cepch.h"
#include "DirectionalLight.h"
#include "CrashEngine/Renderer/RenderCommand.h"
#include "CrashEngine/Core/Defines.h"


namespace CrashEngine
{
	CrashEngine::DirectionalLight::DirectionalLight()
	{
		depthMapShader = Shader::Create("depthMap.vert", "depthMap.frag");

		FramebufferSpecification depthspec;
		depthspec.Height = 1024;
		depthspec.Width = 1024;

		for (int i = 0; i < 3; i++)
		{
			depthMap.push_back(DepthTexture::Create(1024, 1024));//todo: another size?
		}

		depthFramebuffer = Framebuffer::Create(depthspec, false);

		m_cascadeEnd.push_back(1.0f);
		m_cascadeEnd.push_back(25.0f);
		m_cascadeEnd.push_back(90.0f);
		m_cascadeEnd.push_back(200.0f);

		mCascadeRadius.push_back(0.0f);
		mCascadeRadius.push_back(0.0f);
		mCascadeRadius.push_back(0.0f);

		mCascadeCenter.push_back(glm::vec3(0.0f));
		mCascadeCenter.push_back(glm::vec3(0.0f));
		mCascadeCenter.push_back(glm::vec3(0.0f));

		mCascadeWorldViewProj.push_back(glm::mat4(0.0f));
		mCascadeWorldViewProj.push_back(glm::mat4(0.0f));
		mCascadeWorldViewProj.push_back(glm::mat4(0.0f));
	}

	void DirectionalLight::DrawCSM()
	{
		// Get Camera and Light information
		glm::vec3 look = camera->m_Front;
		glm::vec3 camPos = camera->GetPosition();
		glm::vec3 halfCascadeVec = glm::vec3(look * 0.5f * m_cascadeEnd[3]);
		glm::vec3 projectionCenter = camPos + halfCascadeVec;

		// Get all information to create the orthographic projection matrix
		glm::vec3 focusPoint = projectionCenter + glm::normalize(rotation)*200.f;
		glm::vec3 rightVec = glm::vec3(1.f, 0.f, 0.f);
		glm::vec3 upVec = glm::normalize(glm::cross(rotation, rightVec));


		lightView = glm::lookAt(projectionCenter, focusPoint, upVec);

		lightOrthoProj = glm::ortho(-20.f, 20.f, -20.f, 20.f, -20.f, 200.f);


		// World Position to ShadowMap Matrix
		glm::mat4 lightViewProj = lightView * lightOrthoProj;

		//-------------calcorthoproj-------------------


		for (int i = 0; i < 3; i++)
		{
			//glm::mat4 lightSpaceMatrix = mCascadeWorldViewProj[i];
			glm::mat4 lightSpaceMatrix = lightViewProj;
			
			depthMapShader->Bind();
			depthMapShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
			pbrTextureShader->Bind();
			pbrTextureShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

			RenderCommand::SetViewport(1024, 1024);
			depthFramebuffer->Bind();
			depthFramebuffer->SetDepthTexture(CE_TEXTURE_2D, depthMap[i]->GetRendererID());
			RenderCommand::Clear();

			m_ActiveScene->DepthRender();
		}
		depthFramebuffer->Unbind();

	}
	glm::vec3 DirectionalLight::CalcSphereCenter(float Near,float Far)
	{
		// Create a bounding sphere around the camera frustum for 360 rotation
		float end = Near + Far;
		glm::vec3 camPos = camera->GetPosition();
		glm::vec3 look = camera->m_Front;
		glm::vec3 sphereCenter = camPos + look * (Near + 0.5f * end);

		return sphereCenter;
	}
	float DirectionalLight::CalcSphereRadius(glm::vec3 sphereCenter, float Far)
	{
		// Create a vector to the frustum far corner
		float aspect = Height / Width;
		float tanFovX = tanf(aspect* camera->fov);
		float tanFovY = tanf(aspect);
		glm::vec3 look = camera->m_Front;
		glm::vec3 right = glm::cross(camera->GetPosition() - camera->m_Front, camera->m_Up - camera->GetPosition());
		glm::vec3 farCorner = look + right * tanFovX + camera->m_Up * tanFovY;

		// Compute the frustumBoundingSphere radius
		glm::vec3 boundVec = camera->GetPosition() + farCorner * Far - sphereCenter;
		float sphereRadius = length(boundVec);
		return sphereRadius;

	}
}