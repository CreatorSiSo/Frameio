#include <glad/glad.h>
#include <stb_image.h>

#include "Platforms/OpenGL/OpenGLTexture.hpp"

namespace Frameio {

OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath) : m_FilePath(filePath)
{
  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
  FR_ASSERT(data, "Failed to load image: \'" + filePath + "\'!");
  m_Width = width;
  m_Height = height;

  glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
  glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

  glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  switch (channels) {
    case 3:
    default:
      glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
      break;
    case 4:
      glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
      break;
  }

  stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
  glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
  glBindTextureUnit(slot, m_RendererID);
}

} // namespace Frameio
