#pragma once

#include "frpch.hpp"

#include "Renderer/RenderCommand.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"

namespace Frameio {

class Renderer {
public:
  static void BeginScene();
  static void EndScene();

  static void Submit(const std::shared_ptr<VertexArray> &vertexArray,
                     const std::shared_ptr<Shader> &shader);

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
};

} // namespace Frameio