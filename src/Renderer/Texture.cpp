#include "Renderer/Texture.hpp"
#include "Platforms/OpenGL/OpenGLTexture.hpp"
#include "Renderer/Renderer.hpp"

namespace Frameio {

Ref<Texture2D> Texture2D::Create(const std::string& filePath)
{
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      FR_ASSERT(false, "RendererAPI::None is currently not supported!")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(filePath);
  }

  FR_ASSERT(false, "Unknown RendererAPI!")
  return nullptr;
}

} // namespace Frameio
