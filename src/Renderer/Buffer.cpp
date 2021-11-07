#include "Buffer.hpp"
#include "Platforms/OpenGL/OpenGLBuffer.hpp"
#include "Renderer/Renderer.hpp"

namespace Frameio {

VertexBuffer *VertexBuffer::Create(uint32_t size, float *vertices) {
  switch (Renderer::GetCurrentAPI()) {
  case RendererAPI::None:
    FR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
    return nullptr;
  case RendererAPI::OpenGL:
    return new OpenGLVertexBuffer(size, vertices);
  }

  FR_CORE_ASSERT(false, "Unknown RendererAPI!")
  return nullptr;
}

IndexBuffer *IndexBuffer::Create(uint32_t count, uint32_t *indices) {
  switch (Renderer::GetCurrentAPI()) {
  case RendererAPI::None:
    FR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
    return nullptr;
  case RendererAPI::OpenGL:
    return new OpenGLIndexBuffer(count, indices);
  }

  FR_CORE_ASSERT(false, "Unknown RendererAPI!")
  return nullptr;
}

} // namespace Frameio
