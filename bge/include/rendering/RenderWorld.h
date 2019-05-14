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

class RenderWorld
{
public:
  RenderWorld();

  void Init();

  void SetEventCallback(const std::function<void(Event&)>& callback);

  void Render(float interpolation);

  Mesh LoadMesh(const std::string& filepath);
  ShaderProgramHandle LoadShader(const std::string& filepath);
  Texture2DHandle LoadTexture2D(const std::string& filepath);

  // Perspective Camera
  uint32 AddCamera(const Vec4i32& viewport, float fov, float near, float far);
  // Orthographic Camera
  uint32 AddCamera(const Vec4i32& viewport, float near, float far);
  // Set the view matrix of an existing camera
  void SetView(uint32 cameraId, Mat4f view);

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

  void OnEvent(Event& event);

private:
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
