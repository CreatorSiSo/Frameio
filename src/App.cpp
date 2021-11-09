#include "frpch.hpp"

#include <glm/glm.hpp>

#include "App.hpp"
#include "ImGui/ImGuiLayer.hpp"
#include "Input/Input.hpp"
#include "Log.hpp"
#include "Renderer/Renderer.hpp"

namespace Frameio {

App *App::s_Instance = nullptr;

App::App() {
  FR_CORE_ASSERT(!s_Instance, "App already exists!");
  s_Instance = this;

  m_Window = std::unique_ptr<Window>(Window::Create());
  m_Window->SetEventCallback(FR_BIND_EVENT_FN(App::OnEvent));

  m_ImGuiLayer = new ImGuiLayer();
  PushOverlay(m_ImGuiLayer);

  // TRIANGLE
  m_TriangleVertexArray.reset(VertexArray::Create());

  float triangleVertices[3 * 7] = {
      // clang-format off
       0.0f,  1.0f, 0.0f, 0.1f, 0.6f, 1.0f, 1.0f,
       1.0f, -1.0f, 0.0f, 0.2f, 0.3f, 0.9f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.4f, 0.1f, 1.0f, 1.0f
      // clang-format on
  };

  std::shared_ptr<VertexBuffer> triangleVertexBuffer;
  triangleVertexBuffer.reset(
      VertexBuffer::Create(sizeof(triangleVertices), triangleVertices));

  BufferLayout layout = {{ShaderDataType::Float3, "a_Position"},
                         {ShaderDataType::Float4, "a_Color"}};

  triangleVertexBuffer->SetLayout(layout);
  m_TriangleVertexArray->AddVertexBuffer(triangleVertexBuffer);

  uint32_t triangleIndices[3] = {0, 1, 2};

  std::shared_ptr<IndexBuffer> triangleIndexBuffer;
  triangleIndexBuffer.reset(IndexBuffer::Create(
      sizeof(triangleIndices) / sizeof(uint32_t), triangleIndices));

  m_TriangleVertexArray->SetIndexBuffer(triangleIndexBuffer);
  // END TRIANGLE

  // SQUARE
  m_SquareVertexArray.reset(VertexArray::Create());

  float squareVertices[4 * 7] = {
      // clang-format off
      -1.0f,  1.0f, 0.0f, 0.7f, 0.3f, 0.2f, 1.0f,
       1.0f,  1.0f, 0.0f, 0.9f, 0.3f, 0.3f, 1.0f,
       1.0f, -1.0f, 0.0f, 0.7f, 0.4f, 0.2f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f
      // clang-format on
  };

  std::shared_ptr<VertexBuffer> squareVertexBuffer;
  squareVertexBuffer.reset(
      VertexBuffer::Create(sizeof(squareVertices), squareVertices));

  squareVertexBuffer->SetLayout(layout);
  m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

  uint32_t squareIndices[6] = {0, 1, 2, 0, 3, 2};

  std::shared_ptr<IndexBuffer> squareIndexBuffer;
  squareIndexBuffer.reset(IndexBuffer::Create(
      sizeof(squareIndices) / sizeof(uint32_t), squareIndices));

  m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);
  // END SQUARE

  std::string vertexSource = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 s_Position;
            out vec4 s_Color;

            void main() {
              s_Position = a_Position;
              s_Color = a_Color;
              gl_Position = vec4(a_Position, 1.0);
            }
          )";

  std::string fragmentSource = R"(
            #version 330 core

            layout(location = 0) out vec4 o_Color;

            in vec3 s_Position;
            in vec4 s_Color;

            void main() {
              // o_Color = vec4(s_Position, 1.0);
              o_Color = s_Color;
            }
          )";

  m_Shader.reset(new Shader(vertexSource, fragmentSource));
}

void App::Run() {
  while (m_Running) {
    RenderCommand::SetClearColor({1.0f, 0.0f, 1.0f, 1.0f});
    RenderCommand::Clear();

    Renderer::BeginScene();
    m_Shader->Bind();

    Renderer::Submit(m_SquareVertexArray);
    Renderer::Submit(m_TriangleVertexArray);

    Renderer::EndScene();

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
