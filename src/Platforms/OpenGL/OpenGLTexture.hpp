#pragma once

#include "frpch.hpp"

#include "Renderer/Texture.hpp"

namespace Frameio {

class OpenGLTexture2D : public Texture2D {
public:
  OpenGLTexture2D(const std::string& filePath);
  virtual ~OpenGLTexture2D();

  virtual void Bind(uint32_t slot) const override;
  inline virtual uint32_t GetWidth() override { return m_Width; }
  inline virtual uint32_t GetHeight() override { return m_Height; }

private:
  std::string m_FilePath;
  uint32_t m_Width, m_Height;
  uint32_t m_RendererID;
};

} // namespace Frameio
