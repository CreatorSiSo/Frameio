#pragma once

#include "frpch.hpp"

#include <GLFW/glfw3.h>

#include "Events/AppEvent.hpp"
#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Platforms/OpenGL/OpenGLContext.hpp"
#include "Window.hpp"

namespace Framio {

class WindowsWindow : public Window {
public:
  WindowsWindow(const WindowProps &props);
  virtual ~WindowsWindow();

  void OnUpdate() override;

  inline unsigned int GetWidth() const override;
  inline unsigned int GetHeight() const override;

  // Window attributes
  inline void SetEventCallback(const EventCallbackFunction &callback) override {
    m_Data.EventCallback = callback;
  }
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

  inline virtual void *GetNativeWindow() const { return m_Window; }

private:
  GLFWwindow *m_Window;
  GraphicsContext *m_Context;

  virtual void Init(const WindowProps &props);
  virtual void Shutdown();

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;

    EventCallbackFunction EventCallback;
  };

  WindowData m_Data;
};

} // namespace Framio