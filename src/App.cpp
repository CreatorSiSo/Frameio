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

  glGenVertexArrays(1, &m_VertexArray);
  glBindVertexArray(m_VertexArray);

  glGenBuffers(1, &m_VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

  float vertices[3 * 3] = {
      // clang-format off
    0.0f, 0.5f, 0.0f,
    0.5f,  -0.5f, 0.0f,
    -0.5f,  -0.5f, 0.0f
      // clang-format on
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), nullptr);

  glGenBuffers(1, &m_IndexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
  unsigned int indecies[3] = {0, 1, 2};

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies,
               GL_STATIC_DRAW);
}

App::~App() {}

void App::Run() {
  while (m_Running) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    // for (Layer *layer : m_LayerStack)
    //   layer->OnUpdate();

    // m_ImGuiLayer->Begin();
    // for (Layer *layer : m_LayerStack)
    //   layer->OnImGuiRender();
    // m_ImGuiLayer->End();

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
