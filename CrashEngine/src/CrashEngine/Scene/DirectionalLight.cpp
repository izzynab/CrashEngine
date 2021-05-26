#include "cepch.h"
#include "DirectionalLight.h"

#include "CrashEngine/Renderer/RenderCommand.h"
#include "CrashEngine/Core/Defines.h"

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
		m_cascadeEnd.push_back(25.0f);
		m_cascadeEnd.push_back(90.0f);
		m_cascadeEnd.push_back(200.0f);

		mCascadeWorldViewProj.push_back(glm::mat4(1));
		mCascadeWorldViewProj.push_back(glm::mat4(1));
		mCascadeWorldViewProj.push_back(glm::mat4(1));
	}


	void DirectionalLight::DrawCSM(Camera* camera,Shader*defferedShader)
	{
		//todo: make shadows

		glm::vec3 up = glm::vec3(0.0f, 1.f, 0.0f);
		glm::vec3 right = glm::vec3(1.0f, 0.f, 0.0f);

		float fov = camera->fov;
		float AspectRatio = camera->ScreenWidth / camera->ScreenHeight;
		float AspectRatio1 = camera->ScreenHeight / camera->ScreenWidth;
		float tanHalfHFOV = tanf(glm::radians(fov / 2.0f));
		float tanHalfVFOV = tanf(glm::radians((fov * AspectRatio1) / 2.0f));


		glm::vec3 rot = glm::vec3(
			glm::cos(rotation.x) * glm::sin(rotation.y),
			glm::sin(rotation.x) * glm::sin(rotation.y),
			glm::cos(rotation.y));

		glm::mat4 projViewMatix = camera->GetProjectionMatrix() * camera->GetViewMatrix();

		//glm::mat4 lightView = glm::lookAt(camera->GetPosition() + rot,camera->GetPosition(),up);

		for (int i = 0; i < 3; i++)
		{
			float dmaxZ = -std::numeric_limits<float>::max();
			float dminZ = std::numeric_limits<float>::max();

			glm::vec3 centroid = glm::vec3(0);

			float xn = m_cascadeEnd[i] * tanHalfHFOV;
			float xf = m_cascadeEnd[i + 1] * tanHalfHFOV;
			float yn = m_cascadeEnd[i] * tanHalfVFOV;
			float yf = m_cascadeEnd[i + 1] * tanHalfVFOV;

			glm::vec3 frustumCorners[8] = {
				// near face
				glm::vec3(xn, yn, m_cascadeEnd[i]),
				glm::vec3(-xn, yn, m_cascadeEnd[i]),
				glm::vec3(xn, -yn, m_cascadeEnd[i]),
				glm::vec3(-xn, -yn, m_cascadeEnd[i]),

				// far face
				glm::vec3(xf, yf, m_cascadeEnd[i + 1]),
				glm::vec3(-xf, yf, m_cascadeEnd[i + 1]),
				glm::vec3(xf, -yf, m_cascadeEnd[i + 1]),
				glm::vec3(-xf, -yf, m_cascadeEnd[i + 1])
			};

			for (int i = 0; i < 8; i++)
			{
				centroid += frustumCorners[i];
				dminZ = glm::min(dminZ, frustumCorners[i].z);
				dmaxZ = glm::max(dmaxZ, frustumCorners[i].z);
			}

			centroid = centroid / 8.f;



			// Go back from the centroid up to max.z - min.z in the direction of light
			glm::vec3 lightDirection = rot;
			//glm::vec3 lightDirection = roatation;
			float distance = dmaxZ - dminZ;
			lightDirection *= distance;
			glm::vec3 lightPosition = centroid;
			lightPosition += lightDirection;

			//float lightAngleX = (float)Math.toDegrees(Math.acos(lightDirection.z));
			//float lightAngleY = (float)Math.toDegrees(Math.asin(lightDirection.x));
			//float lightAngleZ = 0;
			glm::mat4 lightView = glm::lookAt(lightPosition, rot, up);

			float minX = std::numeric_limits<float>::max();
			float maxX = -std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float maxY = -std::numeric_limits<float>::max();
			float minZ = std::numeric_limits<float>::max();
			float maxZ = -std::numeric_limits<float>::max();

			for (int i = 0; i < 8; i++)
			{
				glm::vec4 corner = glm::vec4(frustumCorners[i],1);
				corner = lightView * corner;

				//frustumCorners[i] = lightView * glm::vec4(frustumCorners[i],1);

				minX = glm::min(minX, corner.x);
				maxX = glm::max(maxX, corner.x);

				minY = glm::min(minY, corner.y);
				maxY = glm::max(maxY, corner.y);

				minZ = glm::min(minZ, corner.z);
				maxZ = glm::max(maxZ, corner.z);
			}

			float distz = maxZ - minZ;;

			mCascadeWorldViewProj[i] = glm::ortho(minX, maxX, minY, maxY, 0.f, distz);

			//near face
			/*Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[0], frustumCorners[1]);
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[2], frustumCorners[3]);
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[0], frustumCorners[2]);
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[1], frustumCorners[3]);

			//far face
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[4], frustumCorners[5]);
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[6], frustumCorners[7]);
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[4], frustumCorners[6]);
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[5], frustumCorners[7]);

			//connection
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[0], frustumCorners[4]);
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[1], frustumCorners[5]);
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[2], frustumCorners[6]);
			Application::Get().GetDebugger().DrawUpdateLine(frustumCorners[3], frustumCorners[7]);


			Application::Get().GetDebugger().DrawUpdatePoint(centroid, glm::vec3(0.7f, 0.2f, 0.4f), 1.f, PointType::Sphere);
			Application::Get().GetDebugger().DrawUpdatePoint(lightPosition, glm::vec3(0.9f, 0.2f, 0.1f), 1.f, PointType::Cube);*/

			//CE_CORE_TRACE("minX: {0} maxX: {1}", minX, maxX);
			//CE_CORE_TRACE("minY: {0} maxY: {1}", minY, maxY);
			//CE_CORE_TRACE("minZ: {0} maxZ: {1}", minZ, maxZ);
		}




		//-------------------------------------------------
		depthMapShader->Bind();
		depthMapShader->SetUniformFloat("near_plane", near_plane);
		depthMapShader->SetUniformFloat("far_plane", far_plane);

		glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, near_plane, far_plane);

		//glm::mat4 lightSpaceMatrix = lightProjection *lightView;
		//------------------------------------------------


		for (int i = 0; i < 3; i++)
		{
			glm::mat4 lightSpaceMatrix =  mCascadeWorldViewProj[i];

			
			depthMapShader->Bind();
			depthMapShader->SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

			defferedShader->Bind();
			defferedShader->SetUniformMat4("lightSpaceMatrix[" + std::to_string(i) + "]", lightSpaceMatrix);
			defferedShader->SetUniformFloat("cascadeEndClipSpace[" + std::to_string(i) + "]", m_cascadeEnd[i+1]);

			RenderCommand::SetViewport(1024, 1024);
			depthFramebuffer->Bind();
			depthFramebuffer->SetTexture(CE_TEXTURE_2D, depthMap[i]->GetRendererID(),0);
			//depthFramebuffer->SetDepthTexture(CE_TEXTURE_2D, depthMap[0]->GetRendererID());
			RenderCommand::SetClearColor(glm::vec4(0, 0.5, 0, 1));
			RenderCommand::Clear();
			//m_ActiveScene->DepthRender(depthMapShader);
		}
		depthFramebuffer->Unbind();

	}

}