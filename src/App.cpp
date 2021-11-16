#include "frpch.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "App.hpp"
#include "Input/Input.hpp"

namespace Frameio {

App *App::s_Instance = nullptr;

App::App() {
  FR_CORE_ASSERT(!s_Instance, "App already exists!");
  s_Instance = this;

  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(FR_BIND_EVENT_FN(App::OnEvent));

  m_ImGuiLayer = new ImGuiLayer();
  PushOverlay(m_ImGuiLayer);
}

void App::Run() {
  while (m_Running) {
    // TODO Move into Platforms/...
    float time = (float)glfwGetTime();
    RealDeltaTime realDeltaTime = time - m_LastFrameTime;
    m_LastFrameTime = time;

    for (Layer *layer : m_LayerStack)
      layer->OnUpdate(realDeltaTime);

    m_ImGuiLayer->Begin();
    for (Layer *layer : m_LayerStack)
      layer->OnImGuiRender();
    m_ImGuiLayer->End();

    m_Window->OnUpdate();
  }
}

void App::PushLayer(Layer *layer) { m_LayerStack.PushLayer(layer); }

void App::PushOverlay(Layer *overlay) { m_LayerStack.PushOverlay(overlay); }

void App::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(FR_BIND_EVENT_FN(App::OnWindowClose));

  //* Debug: Log all events
  // FR_CORE_TRACE("{0}", e);

  for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
    (*--it)->OnEvent(e);
    if (e.Handled)
      break;
  }
}

bool App::OnWindowClose(WindowCloseEvent &e) {
  m_Running = false;
  return true;
}

} // namespace Frameio
