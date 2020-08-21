#include "cepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace CrashEngine {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}