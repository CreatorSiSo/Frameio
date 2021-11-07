#pragma once

#include "frpch.hpp"

#include "Layer.hpp"

namespace Framio {

class ImGuiLayer : public Layer {
public:
  ImGuiLayer();
  ~ImGuiLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnImGuiRender() override;

  void Begin();
  void End();

private:
  float m_Time = 0.0f;
};

} // namespace Framio
