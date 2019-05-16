#pragma once

#include "Mesh.h"
#include "RenderDevice.h"

#include "math/Transform.h"

namespace bge
{

/**
 * Handles wireframe rendering of collider spheres
 */
class WireframeSphereRenderer
{
public:
  /**
   * Renders all existing collider spheres from the POV of the input camera
   * @param projection the camera's projection matrix
   * @param view the camera's view matrix
   */
  void RenderWireframes(const Mat4f& projection, const Mat4f& view);

  /**
   * Sets the mesh which is used for rendering to the collider's location.
   * Most accurate mesh would be a sphere in this case.
   * @param mesh the mesh to render
   */
  void SetMesh(Mesh mesh);

  /**
   * Sets the shader to use for rendering of the collider boxes
   * @param shader the shader to use
   */
  void SetShader(ShaderProgramHandle shader);

  /**
   * Enables or disables the rendering of wireframe boxes
   * @param enabled the flag to set
   */
  FORCEINLINE void SetEnabled(bool enabled) { m_IsEnabled = enabled; }

private:
  Mesh m_SphereMesh;                     /**< The mesh to use for rendering */
  ShaderProgramHandle m_WireframeShader; /**< shader for rendering */

  bool m_IsEnabled = false; /**< enabled rendering flag */
};

} // namespace bge