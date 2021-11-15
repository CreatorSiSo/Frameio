#pragma once

#include "frpch.hpp"

#include "Events/AppEvent.hpp"
#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "LayerStack.hpp"
#include "Renderer/Buffer.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"
#include "Window.hpp"

namespace Frameio {

class App {
public:
  App();
  virtual ~App() = default;

  void Run();

  void OnEvent(Event &e);

  void PushLayer(Layer *layer);
  void PushOverlay(Layer *overlay);

  inline static App &GetApp() { return *s_Instance; }
  inline Window &GetWindow() { return *m_Window; }

private:
  bool OnWindowClose(WindowCloseEvent &e);

private:
  std::unique_ptr<Window> m_Window;
  LayerStack m_LayerStack;
  ImGuiLayer *m_ImGuiLayer;

  bool m_Running = true;

  std::shared_ptr<Shader> m_Shader;
  std::shared_ptr<Shader> m_ShaderPos;
  std::shared_ptr<VertexArray> m_TriangleVertexArray;
  std::shared_ptr<VertexArray> m_SquareVertexArray;

  OrthographicCamera m_Camera;

private:
  static App *s_Instance;
};

// Defined by client
App *CreateApp();

} // namespace Frameio
