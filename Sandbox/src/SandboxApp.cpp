
#include <Crash.h>

#include "imgui/imgui.h"

#include "CrashEngine/Renderer/Shader.h"
#include "CrashEngine/Renderer/Buffer.h"
#include "CrashEngine/Renderer/VertexArray.h"
#include "CrashEngine/Renderer/Renderer.h"
#include "CrashEngine/Renderer/Camera.h"
#include "CrashEngine/Renderer/Texture.h"
#include "CrashEngine/Renderer/Framebuffer.h"

#include "CrashEngine/Renderer/Model.h"
#include "CrashEngine/Renderer/BasicShapes.h"

#include "CrashEngine/Input.h"
#include "CrashEngine/Window.h"
#include "CrashEngine/FileManager.h"
#include "CrashEngine/Renderer/TextRenderer.h"

#include "CrashEngine/Application.h"

namespace CrashEngine {
	class ExampleLayer : public CrashEngine::Layer
	{
	public:
		ExampleLayer()
			: Layer("Example")
		{
			//auto window = Application::Get().GetWindow();

			float Height = Application::Get().GetWindow().GetHeight();
			float Width = Application::Get().GetWindow().GetWidth();

			camera.reset(new Camera(glm::vec3(0.0f, 0.0f, 3.0f), Width, Height));
			camera->CameraSpeed = 4.f;

			projection = glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 100.0f);
			

			pbrShader = Shader::Create("pbr.vert", "pbr.frag");
			pbrShader->Bind();

			pbrShader->SetUniformMat4("projection", projection);
			pbrShader->SetUniformVec3("albedo", glm::vec3(1.0f, 0.0f, 1.0f));
			pbrShader->SetUniformFloat("ao", 1.0f);
			pbrShader->SetUniformInt("irradianceMap", 0);
			pbrShader->SetUniformInt("prefilterMap", 1);
			pbrShader->SetUniformInt("brdfLUT", 2);

			equirectangularToCubemapShader = Shader::Create("cubemap.vert","equirectangularToCubemap.frag");
			irradianceShader = Shader::Create("cubemap.vert","irradiance.frag");
			prefilterShader = Shader::Create("cubemap.vert","prefilter.frag");
			brdfShader = Shader::Create("brdf.vert","brdf.frag");

			backgroundShader = Shader::Create("background.vert","background.frag");
			backgroundShader->Bind();
			backgroundShader->SetUniformInt("environmentMap", 0);

			basicShader = Shader::Create("Basic.vert", "Basic.frag");


			pbrTextureShader = Shader::Create("pbr.vert", "pbrTexture.frag");
			pbrTextureShader->Bind();


			pbrTextureShader->SetUniformMat4("projection", projection);
			pbrTextureShader->SetUniformInt("albedoMap", 0);
			pbrTextureShader->SetUniformInt("normalMap", 1);
			pbrTextureShader->SetUniformInt("metallicMap", 2);
			pbrTextureShader->SetUniformInt("roughnessMap", 3);
			pbrTextureShader->SetUniformInt("aoMap", 4);

			pbrTextureShader->SetUniformInt("irradianceMap", 5);
			pbrTextureShader->SetUniformInt("prefilterMap", 6);
			pbrTextureShader->SetUniformInt("brdfLUT", 7);


			
			albedo = Texture2D::Create("cerberus\\cerberus_A.tga");
			normal = Texture2D::Create("gold\\normal.png");
			metallic = Texture2D::Create("gold\\metallic.png");
			roughness = Texture2D::Create("gold\\roughness.png");
			ao = Texture2D::Create("gold\\ao.jpg");

			/*albedo = Texture2D::Create("cerberus\\Cerberus_A.tga");
			normal = Texture2D::Create("cerberus\\Cerberus_N.tga");
			metallic = Texture2D::Create("cerberus\\Cerberus_M.tga");
			roughness = Texture2D::Create("cerberus\\Cerberus_R.tga");
			ao = Texture2D::Create("cerberus\\Cerberus_AO.tga");*/

			gun.reset(Model::Create("C:\\EngineDev\\CrashEngine\\Models\\cerberus\\cerberus.obj", TextureType::TGA));

			// pbr: setup framebuffer
			FramebufferSpecification spec;
			spec.Height = 1080;
			spec.Width = 2560;

			renderFramebuffer = Framebuffer::Create(spec);

			Framebuffer = Framebuffer::Create(spec);
			Framebuffer->Bind();


			Renderbuffer = Renderbuffer::Create();
			Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, 1920, 1080);
			Renderbuffer->AttachToFramebuffer();



			// initialize default shapes
			sphere.reset(new Sphere());
			square.reset(new Square());
			quad.reset(new Quad());


			// set default settings
			RenderCommand::Enable(CE_DEPTH_TEST);
			RenderCommand::Enable(CE_TEXTURE_CUBE_MAP_SEAMLESS);
			RenderCommand::DepthFunc(CE_LEQUAL);


			// pbr: load the HDR environment map
			HDR = TextureHDR::Create("hdr\\14-Hamarikyu_Bridge_B_3k.hdr");

			// pbr: setup cubemap to render to and attach to framebuffer
			Cubemap = CubemapTexture::Create(1080, 1080, true);


			// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
			glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
			glm::mat4 captureViews[] =
			{
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			};


			// pbr: convert HDR equirectangular environment map to cubemap equivalent
			equirectangularToCubemapShader->Bind();
			equirectangularToCubemapShader->SetUniformInt("equirectangularMap", 0);
			equirectangularToCubemapShader->SetUniformMat4("projection", captureProjection);
			RenderCommand::BindTexture(HDR->GetRendererID(), 0);

			RenderCommand::SetViewport(1080, 1080);
			Framebuffer->Bind();
			for (unsigned int i = 0; i < 6; ++i)
			{
				equirectangularToCubemapShader->SetUniformMat4("view", captureViews[i]);
				Framebuffer->SetTexture(CE_TEXTURE_CUBE_MAP_POSITIVE_X + i, Cubemap->GetRendererID(),0);
				RenderCommand::Clear();

				square->RenderSquare();
			}
			Framebuffer->Unbind();

			// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
			Cubemap->CreateMipmap();

			// pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
			Irradiancemap = CubemapTexture::Create(32, 32, false);//TODO: more that 512 pixels make irradiance map wrong because when there is more than 512 cubemap is scaled and irrradiance map captures smaller section of cubemap
			Framebuffer->Bind();
			Renderbuffer->Bind();
			Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, 32, 32);


			// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
			irradianceShader->Bind();
			irradianceShader->SetUniformInt("environmentMap", 0);
			irradianceShader->SetUniformMat4("projection", captureProjection);
			RenderCommand::BindCubemap(Cubemap->GetRendererID(),0);


			RenderCommand::SetViewport(32, 32); // don't forget to configure the viewport to the capture dimensions.
			Framebuffer->Bind();
			for (unsigned int i = 0; i < 6; ++i)
			{
				irradianceShader->SetUniformMat4("view", captureViews[i]);
				Framebuffer->SetTexture(CE_TEXTURE_CUBE_MAP_POSITIVE_X + i, Irradiancemap->GetRendererID() , 0);
				RenderCommand::Clear();
				square->RenderSquare();
			}
			Framebuffer->Unbind();
	


			// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
			Prefiltermap = CubemapTexture::Create(128, 128, true);
			Prefiltermap->CreateMipmap();

			// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
			prefilterShader->Bind();
			prefilterShader->SetUniformInt("environmentMap", 0);
			prefilterShader->SetUniformMat4("projection", captureProjection);
			RenderCommand::BindCubemap(Cubemap->GetRendererID(), 0);

			Framebuffer->Bind();
			unsigned int maxMipLevels = 5;
			for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
			{
				// reisze framebuffer according to mip-level size.
				unsigned int mipWidth = 128 * std::pow(0.5, mip);
				unsigned int mipHeight = 128 * std::pow(0.5, mip);
				Renderbuffer->Bind();
				Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, mipWidth, mipHeight);
				RenderCommand::SetViewport(mipWidth, mipHeight);

				float roughness = (float)mip / (float)(maxMipLevels - 1);
				prefilterShader->SetUniformFloat("roughness", roughness);
				for (unsigned int i = 0; i < 6; ++i)
				{
					prefilterShader->SetUniformMat4("view", captureViews[i]);
					Framebuffer->SetTexture(CE_TEXTURE_CUBE_MAP_POSITIVE_X + i, Prefiltermap->GetRendererID(), mip);
	
					RenderCommand::Clear();
					square->RenderSquare();
				}
			}
			Framebuffer->Unbind();

			// pbr: generate a 2D LUT from the BRDF equations used.
			brdfLUTTexture = Texture2D::Create(512, 512);

			// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
			Framebuffer->Bind();
			Renderbuffer->Bind();
			Renderbuffer->SetStorage(CE_DEPTH_COMPONENT24, 512, 512);
			Framebuffer->SetTexture(CE_TEXTURE_2D, brdfLUTTexture->GetRendererID(), 0);

			RenderCommand::SetViewport(512, 512);
			brdfShader->Bind();
			RenderCommand::Clear();
			quad->RenderQuad();

			Framebuffer->Unbind();
			


			// initialize static shader uniforms before rendering
			pbrShader->Bind();
			pbrShader->SetUniformMat4("projection", projection);
			backgroundShader->Bind();
			backgroundShader->SetUniformMat4("projection", projection);

			UniformBufferLayout uniformLayout = {
			{ ShaderDataType::Mat4, "projection" },
			{ ShaderDataType::Mat4, "view"},
			};

			m_MatrixUB.reset(UniformBuffer::Create(uniformLayout,0));
			m_MatrixUB->linkShader(pbrShader->GetID(), "matricies");
			m_MatrixUB->linkShader(pbrTextureShader->GetID(), "matricies");
			m_MatrixUB->linkShader(backgroundShader->GetID(), "matricies");

			m_MatrixUB->setData("projection", glm::value_ptr(projection));
			m_MatrixUB->setData("view", glm::value_ptr(projection));



			RenderCommand::SetViewport(Width, Height);
		}

		void OnUpdate() override
		{
			camera->Update();

			renderFramebuffer->Bind();

			RenderCommand::Clear();
			RenderCommand::SetClearColor({ 1.f, 0.f, 0.0f, 1.0f });

			Renderer::BeginScene();

			view = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
			
			pbrTextureShader->Bind();
			pbrTextureShader->SetUniformMat4("view", view);
			pbrTextureShader->SetUniformVec3("camPos", camera->Position);


			RenderCommand::BindTexture(albedo->GetRendererID(), 0);
			RenderCommand::BindTexture(normal->GetRendererID(), 1);
			RenderCommand::BindTexture(metallic->GetRendererID(), 2);
			RenderCommand::BindTexture(roughness->GetRendererID(), 3);
			RenderCommand::BindTexture(ao->GetRendererID(), 4);

			RenderCommand::BindCubemap(Irradiancemap->GetRendererID(), 5);
			RenderCommand::BindCubemap(Prefiltermap->GetRendererID(), 6);
			RenderCommand::BindTexture(brdfLUTTexture->GetRendererID(), 7);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(4, 1, 1));
			pbrTextureShader->SetUniformMat4("model", model);
			//gun->Draw(pbrTextureShader);
			sphere->RenderSphere();

			pbrShader->Bind();
			pbrShader->SetUniformMat4("view", view);
			pbrShader->SetUniformVec3("camPos", camera->Position);

			RenderCommand::BindCubemap(Irradiancemap->GetRendererID(), 0);
			RenderCommand::BindCubemap(Prefiltermap->GetRendererID(), 1);
			RenderCommand::BindTexture(brdfLUTTexture->GetRendererID(), 2);

			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(1, 1, 1));
			pbrShader->SetUniformMat4("model", model);

			// render rows*column number of spheres with material properties defined by textures (they all have the same material properties)
			glm::mat4 model = glm::mat4(1.0f);
			for (int row = 0; row < nrRows; ++row)
			{
				pbrShader->SetUniformFloat("metallic", (float)row / (float)nrRows);
				for (int col = 0; col < nrColumns; ++col)
				{
					pbrShader->SetUniformFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));//TODO: when rougness is low normal doesnt work and lighting dont make good spheres
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(
						(float)(col - (nrColumns / 2)) * spacing,
						(float)(row - (nrRows / 2)) * spacing,
						0.0f
					));
					pbrShader->SetUniformMat4("model", model);
					sphere->RenderSphere();
				}
			}

			// render light source (simply re-render sphere at light positions)
			// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
			// keeps the codeprint small.
			for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
			{
				glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(RenderCommand::GetTime() * 5.0) * 5.0, 0.0, 0.0);
				newPos = lightPositions[i];
				pbrShader->Bind();
				pbrShader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
				pbrShader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
				model = glm::mat4(1.0f);
				model = glm::translate(model, newPos);
				model = glm::scale(model, glm::vec3(0.5f));
				pbrShader->SetUniformMat4("model", model);
				//sphere->RenderSphere();

				pbrTextureShader->Bind();
				pbrTextureShader->SetUniformVec3("lightPositions[" + std::to_string(i) + "]", newPos);
				pbrTextureShader->SetUniformVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
			}

			
			// render skybox (render as last to prevent overdraw)
			backgroundShader->Bind();
			backgroundShader->SetUniformMat4("view", view);
			RenderCommand::BindCubemap(Cubemap->GetRendererID(), 0);
			//RenderCommand::BindCubemap(Irradiancemap->GetRendererID(), 0); // display irradiance map used to diffuse light
			//RenderCommand::BindCubemap(Prefiltermap->GetRendererID(), 0); // display prefilter map used to specular light

			square->RenderSquare();

			Renderer::EndScene();
			renderFramebuffer->Unbind();

			RenderCommand::Clear();
			RenderCommand::SetClearColor(glm::vec4(1,1,0,1));
			basicShader->Bind();
			RenderCommand::BindTexture(renderFramebuffer->GetColorAttachmentRendererID(), 0);
			quad->RenderQuad();
		}

		virtual void OnImGuiRender() override
		{
			bool s = true;
			bool* show = &s;

			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->GetWorkPos());
				ImGui::SetNextWindowSize(viewport->GetWorkSize());
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", show, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);


			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);



				ImGui::Begin("Render context");
				ImVec2 WindowView = ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 40);

				
				ImGui::Image((void*)renderFramebuffer->GetColorAttachmentRendererID(), WindowView,ImVec2(0,1),ImVec2(1,0));
				//renderFramebuffer->Resize(WindowView.y, WindowView.x); //this make screen black
				renderFramebuffer->SetNewTexture(WindowView.x, WindowView.y);

				RenderCommand::SetViewport(WindowView.x, WindowView.y);

				projection = glm::perspective(glm::radians(45.0f), (float)WindowView.x / (float)WindowView.y, 0.1f, 100.0f);		

				pbrTextureShader->Bind();
				pbrTextureShader->SetUniformMat4("projection", projection);

				pbrShader->Bind();
				pbrShader->SetUniformMat4("projection", projection);

				backgroundShader->Bind();
				backgroundShader->SetUniformMat4("projection", projection);

				ImGui::End();
			}
			else
			{
				//ShowDockingDisabledMessage();
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Docking"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows,
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

					if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
					if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
					if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
					if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
					if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
					ImGui::Separator();
					if (ImGui::MenuItem("Close DockSpace", NULL, false, show != NULL))
						*show = false;
					ImGui::EndMenu();
				}


				ImGui::EndMenuBar();
			}

			ImGui::End();
		}

		void OnEvent(CrashEngine::Event& event) override
		{
			if (event.GetEventType() == CrashEngine::EventType::WindowResize)
			{
				CrashEngine::WindowResizeEvent& e = (CrashEngine::WindowResizeEvent&)event;
				camera->SetHeight(e.GetHeight());
				camera->SetWidth(e.GetWidth());

				//projection = glm::perspective(glm::radians(45.0f), (float)e.GetWidth() / (float)e.GetHeight(), 0.1f, 100.0f);
				//
				//pbrTextureShader->Bind();
				//pbrTextureShader->SetUniformMat4("projection", projection);

				//RenderCommand::SetViewport(e.GetWidth(), e.GetHeight());
			}

			if (event.GetEventType() == CrashEngine::EventType::MouseMoved)
			{
				CrashEngine::MouseMovedEvent& e = (CrashEngine::MouseMovedEvent&)event;

			}

		}

	private:
		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<UniformBuffer> m_MatrixUB;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<Model> backpack;
		std::shared_ptr<Model> gun;
		std::shared_ptr<TextRenderer> textRenderer;

		Shader* basicShader;

		Shader* pbrShader;
		Shader* pbrTextureShader;
		Shader* equirectangularToCubemapShader;
		Shader* irradianceShader;
		Shader* prefilterShader;
		Shader* brdfShader;
		Shader* backgroundShader;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		double lastTime = 0;
	
		// lights
		glm::vec3 lightPositions[12] = {
			glm::vec3(-10.0f,  10.0f, 10.0f),
			glm::vec3(10.0f,  10.0f, 10.0f),
			glm::vec3(-10.0f, -10.0f, 10.0f),
			glm::vec3(10.0f, -10.0f, 10.0f),
		};
		glm::vec3 lightColors[12] = {
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

		std::shared_ptr<Texture2D> albedo;
		std::shared_ptr<Texture2D> normal;
		std::shared_ptr<Texture2D> metallic;
		std::shared_ptr<Texture2D> roughness;
		std::shared_ptr<Texture2D> ao;

		std::shared_ptr<Texture2D> albedo1;
		std::shared_ptr<Texture2D> normal1;
		std::shared_ptr<Texture2D> metallic1;
		std::shared_ptr<Texture2D> roughness1;
		std::shared_ptr<Texture2D> ao1;

		std::shared_ptr<TextureHDR> HDR;
		std::shared_ptr<CubemapTexture> Cubemap;
		std::shared_ptr<CubemapTexture> Irradiancemap;
		std::shared_ptr<CubemapTexture> Prefiltermap;
		std::shared_ptr<Texture2D> brdfLUTTexture;


		std::shared_ptr<Framebuffer> renderFramebuffer;

		std::shared_ptr<Framebuffer> Framebuffer;
		std::shared_ptr<Renderbuffer> Renderbuffer;
	};

	class Sandbox : public CrashEngine::Application
	{
	public:
		Sandbox()
		{
			PushLayer(new ExampleLayer());

		}
		~Sandbox()
		{
		}
	};

	CrashEngine::Application* CrashEngine::CreateApplication()
	{
		return new Sandbox;
	}
}