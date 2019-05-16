#pragma once

#include "RenderDevice.h"

namespace bge
{

/**
 * Mesh struct used for rendering
 */
struct Mesh
{
  VertexArrayHandle m_VertexArray;
  VertexBufferHandle m_VertexBuffer;
  IndexBufferHandle m_IndexBuffer;
  uint16 m_IndicesCount;
};

} // namespace bge