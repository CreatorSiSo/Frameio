#include <glad/glad.h>

#include "Platforms/OpenGL/OpenGLVertexArray.hpp"

namespace Frameio {

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
  switch (type) {
    case ShaderDataType::None:
      FR_CORE_ASSERT(false, "Your ShaderDataType is None which has no type!")
      return GL_NONE;
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
      return GL_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
  }

  FR_CORE_ASSERT(false, "Unknow ShaderDataType!")
  return 0;
}

OpenGLVertexArray::OpenGLVertexArray()
{
  glGenVertexArrays(1, &m_RendererID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
  glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArray::Bind() const
{
  glBindVertexArray(m_RendererID);
}

void OpenGLVertexArray::Unbind() const
{
  glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
  FR_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout!")

  glBindVertexArray(m_RendererID);
  vertexBuffer->Bind();

  uint32_t index = 0;
  const auto& layout = vertexBuffer->GetLayout();
  for (const auto& element : layout) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index,
                          element.GetComponentCount(),
                          ShaderDataTypeToOpenGLBaseType(element.Type),
                          element.Normalized ? GL_TRUE : GL_FALSE,
                          layout.GetStride(),
                          TO_CONST_VOID_P(element.Offset));
    index++;
  }

  m_VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
  glBindVertexArray(m_RendererID);
  indexBuffer->Bind();

  m_IndexBuffer = indexBuffer;
}

} // namespace Frameio
