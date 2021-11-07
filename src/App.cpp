#include "frpch.hpp"

//! TODO Temprorary
#include <glad/glad.h>

#include "App.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "Input/Input.hpp"
#include "Log.hpp"

namespace Framio {

App *App::s_Instance = nullptr;

App::App() {
  FR_CORE_ASSERT(!s_Instance, "App already exists!");
  s_Instance = this;

  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(FR_BIND_EVENT_FN(App::OnEvent));

  m_ImGuiLayer = new ImGuiLayer();
  PushOverlay(m_ImGuiLayer);
}

App::~App() {}

void App::Run() {
  while (m_Running) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (Layer *layer : m_LayerStack)
      layer->OnUpdate();

    m_ImGuiLayer->Begin();
    for (Layer *layer : m_LayerStack)
      layer->OnImGuiRender();
    m_ImGuiLayer->End();

    m_Window->OnUpdate();
  }
}

void App::PushLayer(Layer *layer) {
  m_LayerStack.PushLayer(layer);
  layer->OnAttach();
}

void App::PushOverlay(Layer *overlay) {
  m_LayerStack.PushOverlay(overlay);
  overlay->OnAttach();
}

void App::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(FR_BIND_EVENT_FN(App::OnWindowClose));

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

} // namespace Framio
