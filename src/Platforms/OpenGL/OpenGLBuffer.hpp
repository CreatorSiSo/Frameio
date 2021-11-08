#pragma once

#include "frpch.hpp"

#include "Renderer/Buffer.hpp"

namespace Frameio {

class OpenGLVertexBuffer : public VertexBuffer {
public:
  OpenGLVertexBuffer(uint32_t size, float *vertices);
  virtual ~OpenGLVertexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual const BufferLayout &GetLayout() const override { return m_Layout; }
  virtual void SetLayout(const BufferLayout &layout) override {
    m_Layout = layout;
  }

private:
  uint32_t m_RendererID;
  BufferLayout m_Layout;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
  OpenGLIndexBuffer(unsigned int count, uint32_t *indices);
  virtual ~OpenGLIndexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;
  virtual uint32_t GetCount() const override { return m_Count; }

private:
  uint32_t m_RendererID;
  uint32_t m_Count;
};

} // namespace Frameio
