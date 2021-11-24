#include "Renderer/Renderer.hpp"

namespace Frameio {

Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

void Renderer::BeginScene(OrthographicCamera& camera) {
  s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform) {
  shader->Bind();
  shader->UploadUniformMat4("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
  shader->UploadUniformMat4("u_TransformMatrix", transform);

  vertexArray->Bind();

  RenderCommand::DrawIndexed(vertexArray);
}

} // namespace Frameio
