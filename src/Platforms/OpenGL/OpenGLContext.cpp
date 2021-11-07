#include "frpch.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "OpenGLContext.hpp"

namespace Framio {
OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
    : m_WindowHandle(windowHandle) {
  FR_CORE_ASSERT(windowHandle, "Window Handle is NULL!")
}

void OpenGLContext::Init() {
  glfwMakeContextCurrent(m_WindowHandle);

  int gladInitialized = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  FR_CORE_ASSERT(gladInitialized, "Failed to intialize GLAD!")
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }

} // namespace Framio