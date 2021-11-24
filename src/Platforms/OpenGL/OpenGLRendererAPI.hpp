#pragma once

#include "frpch.hpp"

#include <glm/glm.hpp>

#include "Renderer/RendererAPI.hpp"

namespace Frameio {

class OpenGLRendererAPI : public RendererAPI {
public:
  virtual void SetClearColor(const glm::vec4& color) override;
  virtual void Clear() override;

  virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
};

} // namespace Frameio