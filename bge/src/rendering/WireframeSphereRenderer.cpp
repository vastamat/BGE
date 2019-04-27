#include "rendering/WireframeSphereRenderer.h"

#include "physics/PhysicsDevice.h"

namespace bge
{

void WireframeSphereRenderer::RenderWireframes(const Mat4f& projection,
                                               const Mat4f& view)
{
  // Set up the render state that's the same for all object
  RenderDevice::BindShaderProgram(m_WireframeShader);
  RenderDevice::SetUniformMat4(m_WireframeShader, "in_Projection", projection);
  RenderDevice::SetUniformMat4(m_WireframeShader, "in_View", view);
  RenderDevice::BindVertexArray(m_SphereMesh.m_VertexArray);
  RenderDevice::BindIndexBuffer(m_SphereMesh.m_IndexBuffer);

  std::vector<Mat4f> boxTransforms =
      PhysicsDevice::GetAllSphereColliderTransforms();

  // Draw call for each box with a unique transform
  for (auto&& transform : boxTransforms)
  {
    RenderDevice::SetUniformMat4(m_WireframeShader, "in_Model", transform,
                                 false);
    RenderDevice::DrawWireframeLines(m_SphereMesh.m_IndicesCount);
  }
}

void WireframeSphereRenderer::SetMesh(Mesh mesh)
{
  m_SphereMesh = std::move(mesh);
}
void WireframeSphereRenderer::SetShader(ShaderProgramHandle shader)
{
  m_WireframeShader = shader;
}

} // namespace bge
