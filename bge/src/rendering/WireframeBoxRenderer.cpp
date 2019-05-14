#include "rendering/WireframeBoxRenderer.h"

#include "physics/PhysicsDevice.h"

namespace bge
{

void WireframeBoxRenderer::RenderWireframes(const Mat4f& projection,
                                            const Mat4f& view)
{
  if (!m_IsEnabled)
  {
    return;
  }

  // Set up the render state that's the same for all object
  RenderDevice::BindShaderProgram(m_WireframeShader);
  RenderDevice::SetUniformMat4(m_WireframeShader, "in_Projection", projection);
  RenderDevice::SetUniformMat4(m_WireframeShader, "in_View", view);
  RenderDevice::BindVertexArray(m_BoxMesh.m_VertexArray);
  RenderDevice::BindIndexBuffer(m_BoxMesh.m_IndexBuffer);

  std::vector<Mat4f> boxTransforms =
      PhysicsDevice::GetAllBoxColliderTransforms();

  // Draw call for each box with a unique transform
  for (auto&& transform : boxTransforms)
  {
    RenderDevice::SetUniformMat4(m_WireframeShader, "in_Model", transform,
                                 false);
    RenderDevice::DrawWireframeLines(m_BoxMesh.m_IndicesCount);
  }
}

void WireframeBoxRenderer::SetMesh(Mesh mesh) { m_BoxMesh = std::move(mesh); }
void WireframeBoxRenderer::SetShader(ShaderProgramHandle shader)
{
  m_WireframeShader = shader;
}

} // namespace bge
