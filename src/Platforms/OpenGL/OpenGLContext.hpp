#pragma once

#include "frpch.hpp"

#include "Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Framio {

class OpenGLContext : public GraphicsContext {
public:
  OpenGLContext(GLFWwindow *windowHandle);

  virtual void Init() override;
  virtual void SwapBuffers() override;

private:
  GLFWwindow *m_WindowHandle;
};

} // namespace Framio