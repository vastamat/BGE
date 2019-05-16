#pragma once

#include "math/Quat.h"

#include <vector>

namespace bge
{

/**
 * Manager for camera data. The data is stored as SoA, because a lot of the data
 * does not need to be accessed when just the view or projection data is needed.
 */
class CameraManager
{
public:
  /**
   * Add a perspective camera
   * @param viewport the viewport of the camera
   * @param fovDeg the field of view for the camera
   * @param near the near value
   * @param far the far value
   * @return the index of the newly allocated camera.
   */
  uint32 AddPerspective(Vec4i32 viewport, float fovDeg, float near, float far);

  /**
   * Add an orthographic camera
   * @param viewport the viewport of the camera
   * @param near the near value
   * @param far the far value
   * @return the index of the newly allocated camera.
   */
  uint32 AddOrthographic(Vec4i32 viewport, float near, float far);

  /**
   * Sets the view matrix of a single camera
   * @param id the id of the camera
   * @param view the view matrix
   */
  FORCEINLINE void SetView(uint32 id, Mat4f view)
  {
    m_View[id] = std::move(view);
  }

  FORCEINLINE std::vector<Mat4f>& GetProjectionMats() { return m_Projection; }

  FORCEINLINE std::vector<Mat4f>& GetViewMats() { return m_View; }

  FORCEINLINE std::vector<Vec4i32>& GetViewports() { return m_Viewport; }

private:
  std::vector<Mat4f> m_Projection; /**< Array of projection matrices */
  std::vector<Mat4f> m_View;       /**< Array of view matrices */

  std::vector<Vec4i32> m_Viewport; /**< Array of viewports */
  std::vector<float> m_FovDegrees; /**< Array of FOVs */
  std::vector<float> m_Near;       /**< Array of near values */
  std::vector<float> m_Far;        /**< Array of far values */

  std::vector<bool> m_IsOrtho; /**< Array of flags for ortho state */
};

} // namespace bge