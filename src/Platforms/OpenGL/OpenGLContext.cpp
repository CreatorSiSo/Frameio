#include <glad/glad.h>

#include "OpenGLContext.hpp"

namespace Frameio {
OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
    : m_WindowHandle(windowHandle) {
  FR_CORE_ASSERT(windowHandle, "Window Handle is NULL!")
}

void OpenGLContext::Init() {
  glfwMakeContextCurrent(m_WindowHandle);

  int gladInitialized = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  FR_CORE_ASSERT(gladInitialized, "Failed to intialize GLAD!")

  FR_CORE_INFO("OpenGL Renderer:");
  FR_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
  FR_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
  FR_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }

} // namespace Frameio