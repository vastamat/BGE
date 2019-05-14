#pragma once

#include "Mesh.h"
#include "RenderDevice.h"

#include "math/Transform.h"

namespace bge
{

class WireframeSphereRenderer
{
public:
  void RenderWireframes(const Mat4f& projection, const Mat4f& view);

  void SetMesh(Mesh mesh);
  void SetShader(ShaderProgramHandle shader);

  FORCEINLINE void SetEnabled(bool enabled) { m_IsEnabled = enabled; }

private:
  Mesh m_SphereMesh;
  ShaderProgramHandle m_WireframeShader;

  bool m_IsEnabled = false;
};

} // namespace bge