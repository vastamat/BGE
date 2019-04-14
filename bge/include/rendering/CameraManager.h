#pragma once

#include "math/Quat.h"

#include <vector>

namespace bge
{

class CameraManager
{
public:
  uint32 AddPerspective(Vec4i32 viewport, float fovDeg, float near, float far);
  uint32 AddOrthographic(Vec4i32 viewport, float near, float far);

  FORCEINLINE void SetView(uint32 id, Mat4f view)
  {
    m_View[id] = std::move(view);
  }

  FORCEINLINE std::vector<Mat4f>& GetProjectionMats() { return m_Projection; }

  FORCEINLINE std::vector<Mat4f>& GetViewMats() { return m_View; }

  FORCEINLINE std::vector<Vec4i32>& GetViewports() { return m_Viewport; }

private:
  std::vector<Mat4f> m_Projection;
  std::vector<Mat4f> m_View;

  std::vector<Vec4i32> m_Viewport;
  std::vector<float> m_FovDegrees;
  std::vector<float> m_Near;
  std::vector<float> m_Far;

  std::vector<bool> m_IsOrtho;
};

} // namespace bge