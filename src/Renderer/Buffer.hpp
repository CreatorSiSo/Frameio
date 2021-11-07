#pragma once

#include "frpch.hpp"

namespace Frameio {

class VertexBuffer {
public:
  virtual ~VertexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  static VertexBuffer *Create(uint32_t size, float *vertices);
};

class IndexBuffer {
public:
  virtual ~IndexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual uint32_t GetCount() const = 0;

  static IndexBuffer *Create(uint32_t count, uint32_t *indices);
};

} // namespace Frameio
