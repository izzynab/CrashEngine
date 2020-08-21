#pragma once

#include "CrashEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace CrashEngine {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}