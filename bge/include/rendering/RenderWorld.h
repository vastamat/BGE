#pragma once

#include "CameraManager.h"
#include "DynamicMeshSystem.h"
#include "MeshLibrary.h"
#include "ShaderLibrary.h"
#include "StaticMeshSystem.h"
#include "Texture2DLibrary.h"
#include "WireframeBoxRenderer.h"
#include "WireframeSphereRenderer.h"

#include "events/ApplicationEvents.h"

namespace bge
{

/**
 * The render world which encompasses the whole rendering module
 */
class RenderWorld
{
public:
  RenderWorld();

  /**
   * Initializes state which needs construction to be completed
   */
  void Init();

  /**
   * Sets the internal event callback variable
   * It's used to broadcast events to the application layer
   * @param callback the function pointer to use to broadcast events
   */
  void SetEventCallback(const std::function<void(Event&)>& callback);

  /**
   * Renders the world
   * @param interpolation shows how close the state is to the next game update
   */
  void Render(float interpolation);

  /**
   * Loads a mesh from a filepath.
   * @param filepath the filepath to the mesh, including the file name
   * @return the loaded mesh
   */
  Mesh LoadMesh(const std::string& filepath);

  /**
   * Loads a shader from a filepath.
   * @param filepath the path to the file including the file name, but excluding
   * the extension as that is added by the API-specific implementation
   * @return the loaded shader handle
   */
  ShaderProgramHandle LoadShader(const std::string& filepath);

  /**
   * Loads a 2D texture from a filepath.
   * @param filepath the filepath to the texture, including the file name
   * @return the loaded texture
   */
  Texture2DHandle LoadTexture2D(const std::string& filepath);

  /**
   * Add a perspective camera
   * @param viewport the viewport of the camera
   * @param fovDeg the field of view for the camera
   * @param near the near value
   * @param far the far value
   * @return the index of the newly allocated camera.
   */
  uint32 AddCamera(const Vec4i32& viewport, float fov, float near, float far);

  /**
   * Add an orthographic camera
   * @param viewport the viewport of the camera
   * @param near the near value
   * @param far the far value
   * @return the index of the newly allocated camera.
   */
  uint32 AddCamera(const Vec4i32& viewport, float near, float far);

  /**
   * Sets the view matrix of a single camera
   * @param id the id of the camera
   * @param view the view matrix
   */
  void SetView(uint32 cameraId, Mat4f view);

  /**
   * Sets the state of wireframe renderers
   * @param enableBoxWireframe flag for wireframe box rendering
   * @param enableSphereWireframe flag for wireframe sphere rendering
   */
  FORCEINLINE void EnableWireframe(bool enableBoxWireframe,
                                   bool enableSphereWireframe)
  {
    m_WireframeBoxRenderer.SetEnabled(enableBoxWireframe);
    m_WireframeSphereRenderer.SetEnabled(enableSphereWireframe);
  }

  FORCEINLINE StaticMeshSystem& GetStaticMeshSystem()
  {
    return m_StaticMeshSystem;
  }
  FORCEINLINE DynamicMeshSystem& GetDynamicMeshSystem()
  {
    return m_DynamicMeshSystem;
  }

  /**
   * Event handler function
   * @param event the propagated event
   */
  void OnEvent(Event& event);

private:
  /**
   * Event handler for the window close event
   * @param event the broadcast event
   */
  bool OnWindowClose(WindowCloseEvent& event);

  // Systems
  StaticMeshSystem m_StaticMeshSystem;
  DynamicMeshSystem m_DynamicMeshSystem;
  WireframeBoxRenderer m_WireframeBoxRenderer;
  WireframeSphereRenderer m_WireframeSphereRenderer;

  // Resource Libraries
  MeshLibrary m_MeshLibrary;
  ShaderLibrary m_ShaderLibrary;
  Texture2DLibrary m_TextureLibrary;

  // Cameras
  CameraManager m_CameraManager;

  // Event Callback used to fire events to the app
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge
