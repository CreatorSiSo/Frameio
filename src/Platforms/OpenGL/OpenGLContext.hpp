#pragma once

#include "frpch.hpp"

#include <GLFW/glfw3.h>

#include "Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Frameio {

class OpenGLContext : public GraphicsContext {
  public:
    OpenGLContext(GLFWwindow* windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;

  private:
    GLFWwindow* m_WindowHandle;
};

} // namespace Frameio