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

  GLenum internalFormat, dataFormat = 0;
  if (channels == 4) {
    internalFormat = GL_RGBA8;
    dataFormat = GL_RGBA;
  } else if (channels == 3) {
    internalFormat = GL_RGB8;
    dataFormat = GL_RGB;
  }
  FR_ASSERT(internalFormat & dataFormat, "This image format is not supported!");

  glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
  glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

  glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

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
