#pragma once

#include "Mesh.h"
#include "RenderDevice.h"

#include "math/Transform.h"

namespace bge
{

class WireframeBoxRenderer
{
public:
  void RenderWireframes(const Mat4f& projection, const Mat4f& view);

  void SetMesh(Mesh mesh);
  void SetShader(ShaderProgramHandle shader);

private:
  Mesh m_BoxMesh;
  ShaderProgramHandle m_WireframeShader;
};

} // namespace bge