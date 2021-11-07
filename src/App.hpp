#pragma once

#include "frpch.hpp"

#include "Events/AppEvent.hpp"
#include "Events/Event.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "LayerStack.hpp"
#include "Window.hpp"

namespace Framio {

class App {
public:
  App();
  virtual ~App();

  void Run();

  void OnEvent(Event &e);

  void PushLayer(Layer *layer);
  void PushOverlay(Layer *overlay);

  inline static App &GetApp() { return *s_Instance; }
  inline Window &GetWindow() { return *m_Window; }

private:
  bool OnWindowClose(WindowCloseEvent &e);

  std::unique_ptr<Window> m_Window;
  LayerStack m_LayerStack;
  ImGuiLayer *m_ImGuiLayer;

  bool m_Running = true;

private:
  static App *s_Instance;
};

// Defined by client
App *CreateApp();

} // namespace Framio
