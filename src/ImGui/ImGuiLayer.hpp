#pragma once

#include "frpch.hpp"

#include "Core/Layer.hpp"

namespace Frameio {

class ImGuiLayer : public Layer {
public:
  ImGuiLayer();
  ~ImGuiLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnImGuiRender() override;

  void Begin();
  void End();
};

} // namespace Frameio
