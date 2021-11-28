#pragma once

#include "frpch.hpp"

#include <glm/glm.hpp>

namespace Frameio {

class Camera {
private:
  virtual void UpdateViewMatrix() = 0;
};

class OrthographicCamera : public Camera {
public:
  OrthographicCamera(float left, float right, float bottom, float top);

  ~OrthographicCamera() = default;

  inline const glm::vec3& GetPosition() { return m_Position; }
  inline void SetPosition(const glm::vec3& position)
  {
    m_Position = position;
    UpdateViewMatrix();
  }

  inline const float& GetRotation() { return m_Rotation; }
  inline void SetRotation(const float& rotation)
  {
    m_Rotation = rotation;
    UpdateViewMatrix();
  }

  inline const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
  inline const glm::mat4& GetViewProjectionMatrix() { return m_ViewProjectionMatrix; }
  inline const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }

private:
  virtual void UpdateViewMatrix();

private:
  glm::mat4 m_ProjectionMatrix = {
    {0.0f, 0.0f, 0.0f, 0.0f}
  };
  glm::mat4 m_ViewProjectionMatrix = {
    {0.0f, 0.0f, 0.0f, 0.0f}
  };
  glm::mat4 m_ViewMatrix = {
    {0.0f, 0.0f, 0.0f, 0.0f}
  };

  glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
  // TODO Rewrite to a 3d orthographic camera (use 3d rotation)
  float m_Rotation = 0.0f;
};

} // namespace Frameio
