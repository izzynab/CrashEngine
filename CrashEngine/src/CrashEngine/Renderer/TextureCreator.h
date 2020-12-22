#pragma once

#include "Texture.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "BasicShapes.h"

namespace CrashEngine {

	class TextureCreator
	{
	public:
		TextureCreator();

		std::shared_ptr<Texture2D> CreateTexture(float Width, float Height, Shader* shader);
		std::shared_ptr<Texture2D> CreateTexture(float Width, float Height, glm::vec3 color);
	private:
		std::shared_ptr<Framebuffer> framebuffer;
		std::shared_ptr<Renderbuffer> renderbuffer;

		std::shared_ptr<Quad> quad;

		Shader* defaultShader;

	};
}