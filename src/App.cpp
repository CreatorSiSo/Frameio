#include "frpch.hpp"

//! TODO Temprorary
#include <glad/glad.h>

#include "App.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "Input/Input.hpp"
#include "Log.hpp"

namespace Frameio {

App *App::s_Instance = nullptr;

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::Float:
  case ShaderDataType::Float2:
  case ShaderDataType::Float3:
  case ShaderDataType::Float4:
  case ShaderDataType::Mat3:
  case ShaderDataType::Mat4:
    return GL_FLOAT;
  case ShaderDataType::Int:
  case ShaderDataType::Int2:
  case ShaderDataType::Int3:
  case ShaderDataType::Int4:
    return GL_INT;
  case ShaderDataType::Bool:
    return GL_BOOL;
  }

  FR_CORE_ASSERT(false, "Unknow ShaderDataType!")
  return 0;
}

App::App() {
  FR_CORE_ASSERT(!s_Instance, "App already exists!");
  s_Instance = this;

  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(FR_BIND_EVENT_FN(App::OnEvent));

  m_ImGuiLayer = new ImGuiLayer();
  PushOverlay(m_ImGuiLayer);

  glGenVertexArrays(1, &m_VertexArray);
  glBindVertexArray(m_VertexArray);

  float vertices[3 * 7] = {
      // clang-format off
        0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
        1.0f,  -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f,  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
      // clang-format on
  };

  m_VertexBuffer.reset(VertexBuffer::Create(sizeof(vertices), vertices));

  {
    BufferLayout layout = {{ShaderDataType::Float3, "a_Position"},
                           {ShaderDataType::Float4, "a_Color"}};

    m_VertexBuffer->SetLayout(layout);
  }

  uint32_t index = 0;
  const auto &layout = m_VertexBuffer->GetLayout();
  for (const auto &element : layout) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, element.GetComponentCount(),
                          ShaderDataTypeToOpenGLBaseType(element.Type),
                          element.Normalized ? GL_TRUE : GL_FALSE,
                          layout.GetStride(), (const void *)element.Offset);
    index++;
  }

  uint32_t indices[3] = {0, 1, 2};

  m_IndexBuffer.reset(
      IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));

  std::string vertexSource = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 s_Position;
            out vec4 s_Color;

            void main() {
              s_Position = a_Position / 2 + 0.5;
              s_Color = a_Color;
              gl_Position = vec4(a_Position / 2, 1.0);
            }
          )";

  std::string fragmentSource = R"(
            #version 330 core

            layout(location = 0) out vec4 o_Color;

            in vec3 s_Position;
            in vec4 s_Color;

            void main() {
              o_Color = vec4(sin(31.415 * s_Position - 1.5), 1.0);
              o_Color = s_Color;
            }
          )";

  m_Shader.reset(new Shader(vertexSource, fragmentSource));
}

App::~App() {}

void App::Run() {
  while (m_Running) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_Shader->Bind();
    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT,
                   nullptr);

    // for (Layer *layer : m_LayerStack)
    //   layer->OnUpdate();

    // m_ImGuiLayer->Begin();
    // for (Layer *layer : m_LayerStack)
    //   layer->OnImGuiRender();
    // m_ImGuiLayer->End();

    m_Window->OnUpdate();
  }
}

void App::PushLayer(Layer *layer) { m_LayerStack.PushLayer(layer); }

void App::PushOverlay(Layer *overlay) { m_LayerStack.PushOverlay(overlay); }

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

} // namespace Frameio
