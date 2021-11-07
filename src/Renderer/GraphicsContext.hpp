#pragma once

#include "frpch.hpp"

namespace Framio {

class GraphicsContext {
public:
  virtual void Init() = 0;
  virtual void SwapBuffers() = 0;

private:
};

} // namespace Framio