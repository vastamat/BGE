#include "rendering/RenderWorld.h"

#include "core/Application.h"

namespace bge
{

RenderWorld::RenderWorld()
{
  // BUG: Gets initialized before window is created
  // Window& window = Application::Get().GetWindow();
  // // Screen camera uses "window" coordinate origin, being top-left
  // m_ScreenCamera.SetOrthographic(
  //     Vec4i32(0, window.GetHeight(), window.GetWidth(), 0), -1.0f, 1.0f);
}

void RenderWorld::SetEventCallback(const std::function<void(Event&)>& callback)
{
  m_EventCallback = callback;
  m_MeshSystem.SetEventCallback(callback);
}

void RenderWorld::Render(float interpolation)
{
  RenderDevice::ClearBuffers(true, true);

  auto& projectionMats = m_CameraManager.GetProjectionMats();
  auto& viewMats = m_CameraManager.GetViewMats();
  auto& viewports = m_CameraManager.GetViewports();

  for (uint32 i = 0; i < projectionMats.size(); i++)
  {
    auto& viewport = viewports[i];

    RenderDevice::SetViewport(viewport[0], viewport[1], viewport[2],
                              viewport[3]);

    m_MeshSystem.RenderMeshes(projectionMats[i], viewMats[i]);
  }
}

uint32 RenderWorld::AddCamera(const Vec4i32& viewport, float fov, float near,
                              float far)
{
  return m_CameraManager.AddPerspective(viewport, fov, near, far);
}

uint32 RenderWorld::AddCamera(const Vec4i32& viewport, float near, float far)
{
  return m_CameraManager.AddOrthographic(viewport, near, far);
}

Mesh RenderWorld::LoadMesh(const std::string& filepath)
{
  return m_MeshLibrary.GetMesh(filepath);
}

ShaderProgramHandle RenderWorld::LoadShader(const std::string& filepath)
{
  return m_ShaderLibrary.GetShader(filepath);
}

Texture2DHandle RenderWorld::LoadTexture2D(const std::string& filepath)
{
  return m_TextureLibrary.GetTexture(filepath);
}

void RenderWorld::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<WindowCloseEvent>(
      BGE_BIND_EVENT_FN(RenderWorld::OnWindowClose));

  m_MeshSystem.OnEvent(event);
}

bool RenderWorld::OnWindowClose(WindowCloseEvent& event)
{
  m_MeshLibrary.ClearLibrary();
  m_ShaderLibrary.ClearLibrary();
  m_TextureLibrary.ClearLibrary();
}

} // namespace bge