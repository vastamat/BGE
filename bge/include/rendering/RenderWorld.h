#pragma once

#include "CameraManager.h"
#include "MeshLibrary.h"
#include "MeshSystem.h"
#include "ShaderLibrary.h"
#include "Texture2DLibrary.h"

namespace bge
{

class RenderWorld
{
public:
  RenderWorld();

  void SetEventCallback(const std::function<void(Event&)>& callback);

  void Render(float interpolation);
  void OnExit();

  Mesh LoadMesh(const std::string& filepath);
  ShaderProgramHandle LoadShader(const std::string& filepath);
  Texture2DHandle LoadTexture2D(const std::string& filepath);

  uint32 AddCamera(const Vec4i32& viewport, float fov, float near, float far);
  uint32 AddCamera(const Vec4i32& viewport, float near, float far);

  FORCEINLINE MeshSystem& GetMeshSystem() { return m_MeshSystem; }

private:
  // Systems
  MeshSystem m_MeshSystem;

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
