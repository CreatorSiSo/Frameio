#pragma once

#include "frpch.hpp"

namespace Frameio {

class Shader {
public:
  virtual ~Shader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  static Ref<Shader> Create(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);

private:
  uint32_t m_RendererID;
};

} // namespace Frameio
