#include "cepch.h"
#include "DirectionalLight.h"
#include "CrashEngine/Renderer/RenderCommand.h"
#include "CrashEngine/Core/Defines.h"


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
		m_cascadeEnd.push_back(25.0f);
		m_cascadeEnd.push_back(90.0f);
		m_cascadeEnd.push_back(200.0f);

		mCascadeWorldViewProj.push_back(glm::mat4(1));
		mCascadeWorldViewProj.push_back(glm::mat4(1));
		mCascadeWorldViewProj.push_back(glm::mat4(1));

		debugLine.reset(new DebugLine());
	}


	void DirectionalLight::DrawCSM()
	{
		glm::vec3 up = glm::vec3(0.0f, 1.f, 0.0f);
		glm::vec3 right = glm::vec3(1.0f, 0.f, 0.0f);

		glm::vec3 rot = glm::vec3(
			glm::cos(rotation.x) * glm::sin(rotation.y),
			glm::sin(rotation.x) * glm::sin(rotation.y),
			glm::cos(rotation.y));

		glm::mat4 lightView = glm::lookAt(camera->GetPosition() + rot,
			camera->GetPosition(),
			up);

		glm::mat4 camInverse = glm::inverse(camera->GetViewMatrix());

		float fov = camera->fov;
		float AspectRatio = Width / Height;


		for (int i = 0; i < 3; i++) 
		{
			float NearHeight = 2 * glm::tan(fov) * m_cascadeEnd[i];
			float NearWidth = NearHeight * AspectRatio;
			glm::vec3 NearCenter = camera->GetPosition() + glm::normalize(rot) * m_cascadeEnd[i];

			float FarHeight = 2 * glm::tan(fov) * m_cascadeEnd[i+1];
			float FarWidth = FarHeight * AspectRatio;
			glm::vec3 FarCenter = camera->GetPosition() + glm::normalize(rot) * m_cascadeEnd[i+1];

			glm::vec3 NearTopLeft =		NearCenter + up * (NearHeight / 2) - right * (NearWidth / 2);
			glm::vec3 NearTopRight =	NearCenter + up * (NearHeight / 2) + right * (NearWidth / 2);
			glm::vec3 NearBottomLeft =	NearCenter - up * (NearHeight / 2) - right * (NearWidth / 2);
			glm::vec3 NearBottomRight = NearCenter - up * (NearHeight / 2) + right * (NearWidth / 2);

			glm::vec3 FarTopLeft =		FarCenter + up * (FarHeight / 2) - right * (FarWidth / 2);
			glm::vec3 FarTopRight =		FarCenter + up * (FarHeight / 2) + right * (FarWidth / 2);
			glm::vec3 FarBottomLeft =	FarCenter - up * (FarHeight / 2) - right * (FarWidth / 2);
			glm::vec3 FarBottomRight =	FarCenter - up * (FarHeight / 2) + right * (FarWidth / 2);

			NearTopLeft = lightView * glm::vec4(NearTopLeft, 1);
			NearTopRight = lightView * glm::vec4(NearTopRight, 1);
			NearBottomLeft = lightView * glm::vec4(NearBottomLeft, 1);
			NearBottomRight = lightView * glm::vec4(NearBottomRight, 1);

			FarTopLeft = lightView * glm::vec4(FarTopLeft, 1);
			FarTopRight = lightView * glm::vec4(FarTopRight, 1);
			FarBottomLeft = lightView * glm::vec4(FarBottomLeft, 1);
			FarBottomRight = lightView * glm::vec4(FarBottomRight, 1);

			glm::vec3 corners[8] = {
				NearTopLeft,
				NearTopRight,
				NearBottomLeft,
				NearBottomRight,

				FarTopLeft,
				FarTopRight,
				FarBottomLeft,
				FarBottomRight
			};

			float minX = 0;
			float maxX = 0;
			float minY = 0;
			float maxY = 0;
			float minZ = 0;
			float maxZ = 0;

			for (int i = 0; i < 8; i++)
			{
				minX = glm::min(minX, corners[i].x);
				maxX = glm::max(maxX, corners[i].x);

				minY = glm::min(minY, corners[i].y);
				maxY = glm::max(maxY, corners[i].y);

				minZ = glm::min(minZ, corners[i].z);
				maxZ = glm::max(maxZ, corners[i].z);
			}

			mCascadeWorldViewProj[i] = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
			//if(i==0)CE_INFO("{0}, {1}, {2}, {3}, {4}, {5}", minX, maxX, minY, maxY, minZ, maxZ);

			debugLine->DrawDebugLine(NearTopLeft, NearTopRight);
			debugLine->DrawDebugLine(NearBottomLeft, NearBottomRight);

			debugLine->DrawDebugLine(NearTopLeft, NearBottomLeft);
			debugLine->DrawDebugLine(NearTopRight, NearBottomRight);
		}


		/*float ar = Height / Width;
		float tanHalfHFOV = tanf(glm::radians(fov / 2.0f));
		float tanHalfVFOV = tanf(glm::radians((fov * ar) / 2.0f));

		for (int i = 0; i < 3; i++) {
			float xn = m_cascadeEnd[i] * tanHalfHFOV;
			float xf = m_cascadeEnd[i + 1] * tanHalfHFOV;
			float yn = m_cascadeEnd[i] * tanHalfVFOV;
			float yf = m_cascadeEnd[i + 1] * tanHalfVFOV;

			glm::vec4 frustumCorners[8] = {
				// near face
				glm::vec4(xn,   yn, m_cascadeEnd[i], 1.0),
				glm::vec4(-xn,  yn, m_cascadeEnd[i], 1.0),
				glm::vec4(xn,  -yn, m_cascadeEnd[i], 1.0),
				glm::vec4(-xn, -yn, m_cascadeEnd[i], 1.0),

				// far face
				glm::vec4(xf,   yf, m_cascadeEnd[i + 1], 1.0),
				glm::vec4(-xf,  yf, m_cascadeEnd[i + 1], 1.0),
				glm::vec4(xf,  -yf, m_cascadeEnd[i + 1], 1.0),
				glm::vec4(-xf, -yf, m_cascadeEnd[i + 1], 1.0)
			};

			glm::vec4 frustumCornersL[8];

			float minX = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::min();
			float minY = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::min();
			float minZ = std::numeric_limits<float>::max();
			float maxZ = std::numeric_limits<float>::min();

			for (int j = 0; j < 8; j++) {
				glm::vec4 vW = camInverse * frustumCorners[j];
				frustumCornersL[j] = lightView * vW;

				minX = glm::min(minX, frustumCornersL[j].x);
				maxX = glm::max(maxX, frustumCornersL[j].x);
				minY = glm::min(minY, frustumCornersL[j].y);
				maxY = glm::max(maxY, frustumCornersL[j].y);
				minZ = glm::min(minZ, frustumCornersL[j].z);
				maxZ = glm::max(maxZ, frustumCornersL[j].z);
			}

			mCascadeWorldViewProj[i] = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
			if (i == 0)CE_INFO("{0}, {1}, {2}, {3}, {4}, {5}", minX, maxX, minY, maxY, minZ, maxZ);
		}*/



		//-------------------------------------------------
		depthMapShader->Bind();
		depthMapShader->SetUniformFloat("near_plane", near_plane);
		depthMapShader->SetUniformFloat("far_plane", far_plane);

		glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, near_plane, far_plane);

		glm::mat4 lightSpaceMatrix = lightProjection *lightView;
		//------------------------------------------------


		for (int i = 0; i < 3; i++)
		{
			//glm::mat4 lightSpaceMatrix =  mCascadeWorldViewProj[i];

			
			depthMapShader->Bind();
			depthMapShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
			pbrTextureShader->Bind();
			pbrTextureShader->SetUniformMat4("lightSpaceMatrix[" + std::to_string(i) + "]", lightSpaceMatrix);
			pbrTextureShader->SetUniformFloat("cascadeEndClipSpace[" + std::to_string(i) + "]", m_cascadeEnd[i+1]);

			RenderCommand::SetViewport(1024, 1024);
			depthFramebuffer->Bind();
			depthFramebuffer->SetTexture(CE_TEXTURE_2D, depthMap[i]->GetRendererID(),0);
			//depthFramebuffer->SetDepthTexture(CE_TEXTURE_2D, depthMap[0]->GetRendererID());
			RenderCommand::SetClearColor(glm::vec4(0, 0.5, 0, 1));
			RenderCommand::Clear();
			m_ActiveScene->DepthRender(depthMapShader);
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