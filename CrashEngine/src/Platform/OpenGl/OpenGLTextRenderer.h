#pragma once

#include "CrashEngine/Renderer/TextRenderer.h"

namespace CrashEngine {

	class OpenGLTextRenderer : public TextRenderer
	{
	public:
		OpenGLTextRenderer();

		virtual void RenderText(std::string text, float x, float y, float scale, glm::vec3 color) override;

	public:
		struct Character {
			unsigned int TextureID;  // ID handle of the glyph texture
			glm::ivec2   Size;       // Size of glyph
			glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
			unsigned int Advance;    // Offset to advance to next glyph
		};

		std::map<char, Character> Characters;

		unsigned int VAO, VBO;
	};
}
