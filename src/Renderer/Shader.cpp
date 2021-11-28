#include "Shader.hpp"
#include "Platforms/OpenGL/OpenGLShader.hpp"
#include "Renderer/Renderer.hpp"

namespace Frameio {

Shader* Shader::Create(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
{
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      FR_ASSERT(false, "RendererAPI::None is currently not supported!")
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLShader(vertexShaderSrc, fragmentShaderSrc);
  }

  FR_ASSERT(false, "Unknown RendererAPI!")
  return nullptr;
}

} // namespace Frameio
