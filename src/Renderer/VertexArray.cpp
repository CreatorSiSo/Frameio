#include "Renderer/VertexArray.hpp"
#include "Platforms/OpenGL/OpenGLVertexArray.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/RendererAPI.hpp"

namespace Frameio {

VertexArray* VertexArray::Create()
{
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            FR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexArray();
    }

    FR_CORE_ASSERT(false, "Unknown RendererAPI!")
    return nullptr;
}

} // namespace Frameio
