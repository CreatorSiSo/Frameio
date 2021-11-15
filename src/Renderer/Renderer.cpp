#include "Renderer/Renderer.hpp"

namespace Frameio {

Renderer::SceneData *Renderer::s_SceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthographicCamera &camera) {
  s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray,
                      const std::shared_ptr<Shader> &shader) {
  shader->Bind();
  shader->UploadUniformMat4("u_ViewProjectionMatrix",
                            s_SceneData->ViewProjectionMatrix);
  vertexArray->Bind();
  RenderCommand::DrawIndexed(vertexArray);
}

} // namespace Frameio
