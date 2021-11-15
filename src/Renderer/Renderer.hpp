#pragma once

#include "frpch.hpp"

#include <glm/glm.hpp>

#include "Renderer/Camera.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"

namespace Frameio {

class Renderer {
public:
  static void BeginScene(OrthographicCamera &camera);
  static void EndScene();

  static void Submit(const std::shared_ptr<VertexArray> &vertexArray,
                     const std::shared_ptr<Shader> &shader);

  inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
  struct SceneData {
    glm::mat4 ViewProjectionMatrix;
  };

  static SceneData *s_SceneData;
};

} // namespace Frameio