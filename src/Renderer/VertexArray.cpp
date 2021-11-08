#include "Renderer/VertexArray.hpp"
#include "Platforms/OpenGL/OpenGLVertexArray.hpp"
#include "Renderer/Renderer.hpp"

namespace Frameio {

VertexArray *VertexArray::Create() {
  switch (Renderer::GetCurrentAPI()) {
  case RendererAPI::None:
    FR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
    return nullptr;
  case RendererAPI::OpenGL:
    return new OpenGLVertexArray();
  }

  FR_CORE_ASSERT(false, "Unknown RendererAPI!")
  return nullptr;
}

} // namespace Frameio
