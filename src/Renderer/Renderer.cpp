#include "Renderer/Renderer.hpp"

namespace Frameio {

void Renderer::BeginScene() {}
void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray,
                      const std::shared_ptr<Shader> &shader) {
  shader->Bind();
  vertexArray->Bind();
  RenderCommand::DrawIndexed(vertexArray);
}

} // namespace Frameio
