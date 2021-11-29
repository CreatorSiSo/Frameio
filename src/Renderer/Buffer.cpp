#include "Renderer/Buffer.hpp"
#include "Platforms/OpenGL/OpenGLBuffer.hpp"
#include "Renderer/Renderer.hpp"

namespace Frameio {

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
{
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      FR_ASSERT(false, "RendererAPI::None is currently not supported!")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLVertexBuffer>(size, vertices);
  }

  FR_ASSERT(false, "Unknown RendererAPI!")
  return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t count, uint32_t* indices)
{
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      FR_ASSERT(false, "RendererAPI::None is currently not supported!")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLIndexBuffer>(count, indices);
  }

  FR_ASSERT(false, "Unknown RendererAPI!")
  return nullptr;
}

} // namespace Frameio
