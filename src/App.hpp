#pragma once

#include "frpch.hpp"

#include "Events/AppEvent.hpp"
#include "Events/Event.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "LayerStack.hpp"
#include "Renderer/Shader.hpp"
#include "Window.hpp"

namespace Frameio {

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

  unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
  std::unique_ptr<Shader> m_Shader;

private:
  static App *s_Instance;
};

// Defined by client
App *CreateApp();

} // namespace Frameio
