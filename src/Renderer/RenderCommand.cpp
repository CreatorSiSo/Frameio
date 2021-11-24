#include "Renderer/RenderCommand.hpp"
#include "Platforms/OpenGL/OpenGLRendererAPI.hpp"

namespace Frameio {

RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

} // namespace Frameio
