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

		FramebufferSpecification depthspec;
		depthspec.Height = 1024;
		depthspec.Width = 1024;

		for (int i = 0; i < 3; i++)
		{
			depthMap.push_back(Texture2D::Create(1024, 1024));//todo: another size?
		}

		depthFramebuffer = Framebuffer::Create(depthspec, true);
		

		m_cascadeEnd.push_back(1.0f);
		m_cascadeEnd.push_back(60.0f);
		m_cascadeEnd.push_back(150.0f);
		m_cascadeEnd.push_back(300.0f);

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
		glm::vec3 right = glm::vec3(1.0f, 0.f, 0.0f);

		float fov = camera->fov;
		float AspectRatio = camera->ScreenWidth / camera->ScreenHeight;

		for (int i = 0; i < 3; i++)
		{
			//splitted frustum
			glm::mat4 cameraProjectionMatrix = glm::perspective(glm::radians(fov), camera->ScreenWidth / camera->ScreenHeight, m_cascadeEnd[0], m_cascadeEnd[i+1]);	
			glm::mat4 cameraProjView = cameraProjectionMatrix * camera->GetViewMatrix();

			Application::Get().GetDebugger().DrawFrustum(cameraProjView);

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

			//calculate light relative to frustum position
			glm::vec3 relativeLightPosition = rot;
			float distance = maxZ - minZ;
			relativeLightPosition *= distance;
			glm::vec3 lightPosition = frustumCenter;
			lightPosition += relativeLightPosition;

			//calculate light view matrix
			float lightAngleX = glm::degrees(glm::acos(rot.z));
			float lightAngleY = glm::degrees(glm::asin(rot.x));
			float lightAngleZ = 0;
			glm::vec3 lightCenter = glm::vec3(lightAngleX, lightAngleY, lightAngleZ);
			glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, lightCenter, up);

			//calculate light projection matrix
			float minX = std::numeric_limits<float>::max();
			float maxX = -std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float maxY = -std::numeric_limits<float>::max();
			minZ = std::numeric_limits<float>::max();
			maxZ = -std::numeric_limits<float>::max();

			for (int i = 0; i < 8; i++) {
				glm::vec3 corner = frustumCorners[i];
				glm::vec4 coords = glm::vec4(corner, 1);

				//coords = coords * lightViewMatrix;
				coords = lightViewMatrix * coords;

				minX = glm::min(coords.x, minX);
				maxX = glm::max(coords.x, maxX);
				minY = glm::min(coords.y, minY);
				maxY = glm::max(coords.y, maxY);
				minZ = glm::min(coords.z, minZ);
				maxZ = glm::max(coords.z, maxZ);
			}
			float distz = maxZ - minZ;

			glm::mat4 lightProjectionMatrix = glm::ortho<float>(minX, maxX, minY, maxY, 0, distz);

			//set mCascadeWorldViewProj
			mCascadeWorldViewProj[i] = lightProjectionMatrix * lightViewMatrix;
			//Application::Get().GetDebugger().DrawFrustum(mCascadeWorldViewProj[i]);
		}
		Application::Get().GetDebugger().DrawFrustum(mCascadeWorldViewProj[0]);


		for (int i = 0; i < 3; i++)
		{
			glm::mat4 lightSpaceMatrix =  mCascadeWorldViewProj[i];

			defferedShader->Bind();
			defferedShader->SetUniformMat4("lightSpaceMatrix[" + std::to_string(i) + "]", lightSpaceMatrix);
			defferedShader->SetUniformFloat("cascadeEndClipSpace[" + std::to_string(i) + "]", m_cascadeEnd[i + 1]);
			
			depthMapShader->Bind();
			depthMapShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

			RenderCommand::SetViewport(1024, 1024);
			depthFramebuffer->Bind();
			depthFramebuffer->SetTexture(CE_TEXTURE_2D, depthMap[i]->GetRendererID(),0);
			RenderCommand::SetClearColor(glm::vec4(0, 0.5, 0, 1));
			RenderCommand::Clear();

			m_ActiveScene->DepthRender(depthMapShader);
		}
		depthFramebuffer->Unbind();


		//Application::Get().GetDebugger().DrawFrustum(mCascadeWorldViewProj[0]);
		//Application::Get().GetDebugger().DrawFrustum(mCascadeWorldViewProj[1]);
		//Application::Get().GetDebugger().DrawFrustum(mCascadeWorldViewProj[2]);
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