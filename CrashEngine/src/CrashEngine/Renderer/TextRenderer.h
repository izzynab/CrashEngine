#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H  

#include <string>
#include <map>
#include <glm/glm.hpp>

#include "CrashEngine/Renderer/Shader.h"

namespace CrashEngine {

	class TextRenderer
	{
	public:
		static TextRenderer* Create();

		virtual void RenderText(std::string text, float x, float y, float scale, glm::vec3 color) = 0;

	};


}