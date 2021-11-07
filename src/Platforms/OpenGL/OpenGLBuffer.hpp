#pragma once

#include "frpch.hpp"

#include "Renderer/Buffer.hpp"

namespace Frameio {

class OpenGLVertexBuffer : public VertexBuffer {
public:
  OpenGLVertexBuffer(uint32_t size, float *vertices);
  virtual ~OpenGLVertexBuffer();

  virtual void Bind() const;
  virtual void Unbind() const;

private:
  uint32_t m_RendererID;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
  OpenGLIndexBuffer(unsigned int count, uint32_t *indices);
  virtual ~OpenGLIndexBuffer();

  virtual void Bind() const;
  virtual void Unbind() const;
  virtual uint32_t GetCount() const { return m_Count; }

private:
  uint32_t m_RendererID;
  uint32_t m_Count;
};

} // namespace Frameio
