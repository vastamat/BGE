#include "CameraControlSystem.h"

#include <core/Application.h>
#include <input/Input.h>
#include <math/Quat.h>

CameraControlSystem::CameraControlSystem(uint32 trackedCameraId)
    : m_TrackedCameraId(trackedCameraId)
{
  bge::Application::Get().GetWindow().SetCursor(false);
}

void CameraControlSystem::Tick(float deltaSeconds)
{
  bge::Quatf orientation =
      (bge::Quatf::GenRotationX(m_Pitch) * bge::Quatf::GenRotationY(m_Yaw)) *
      0.05f;

  bge::Quatf conjugated = orientation.GetConjugated();

  bge::Vec3f zAxis =
      bge::Quatf::RotateVec(conjugated, bge::Vec3f(0.0f, 0.0f, -1.0f));
  bge::Vec3f xAxis =
      bge::Quatf::RotateVec(conjugated, bge::Vec3f(1.0f, 0.0f, 0.0f));
  bge::Vec3f yAxis(0.0f, 1.0f, 0.0f);

  bge::Vec3f translationAmount =
      (zAxis * m_Forward) + (xAxis * m_Right) + (yAxis * m_Up * 0.001f);

  m_Position += translationAmount * 200;

  bge::Mat4f view =
      orientation.ToMat4() * bge::GenTranslationMat(m_Position * -1);

  bge::Application::Get().GetWorld().GetRenderWorld().SetView(m_TrackedCameraId,
                                                              std::move(view));
}

void CameraControlSystem::OnEvent(bge::Event& event)
{
  bge::EventDispatcher dispatcher(event);

  dispatcher.Dispatch<bge::KeyPressedEvent>(
      BGE_BIND_EVENT_FN(CameraControlSystem::OnKeyPressEvent));
  dispatcher.Dispatch<bge::KeyReleasedEvent>(
      BGE_BIND_EVENT_FN(CameraControlSystem::OnKeyReleaseEvent));
  dispatcher.Dispatch<bge::MouseMovedEvent>(
      BGE_BIND_EVENT_FN(CameraControlSystem::OnMouseMoveEvent));
}

bool CameraControlSystem::OnKeyPressEvent(bge::KeyPressedEvent& event)
{
  bge::KeyCode key = event.GetKeyCode();

  if (key == bge::KeyCode::W)
  {
    m_Forward = bge::Clamp(m_Forward + 1.0f, -1.0f, 1.0f);
  }
  if (key == bge::KeyCode::S)
  {
    m_Forward = bge::Clamp(m_Forward - 1.0f, -1.0f, 1.0f);
  }

  if (key == bge::KeyCode::D)
  {
    m_Right = bge::Clamp(m_Right + 1.0f, -1.0f, 1.0f);
  }
  if (key == bge::KeyCode::A)
  {
    m_Right = bge::Clamp(m_Right - 1.0f, -1.0f, 1.0f);
  }

  if (key == bge::KeyCode::Q)
  {
    m_Up = bge::Clamp(m_Up + 1.0f, -1.0f, 1.0f);
  }
  if (key == bge::KeyCode::E)
  {
    m_Up = bge::Clamp(m_Up - 1.0f, -1.0f, 1.0f);
  }

  return false;
}
bool CameraControlSystem::OnKeyReleaseEvent(bge::KeyReleasedEvent& event)
{
  bge::KeyCode key = event.GetKeyCode();

  if (key == bge::KeyCode::W)
  {
    m_Forward = bge::Clamp(m_Forward - 1.0f, -1.0f, 1.0f);
  }
  if (key == bge::KeyCode::S)
  {
    m_Forward = bge::Clamp(m_Forward + 1.0f, -1.0f, 1.0f);
  }

  if (key == bge::KeyCode::D)
  {
    m_Right = bge::Clamp(m_Right - 1.0f, -1.0f, 1.0f);
  }
  if (key == bge::KeyCode::A)
  {
    m_Right = bge::Clamp(m_Right + 1.0f, -1.0f, 1.0f);
  }

  if (key == bge::KeyCode::Q)
  {
    m_Up = bge::Clamp(m_Up - 1.0f, -1.0f, 1.0f);
  }
  if (key == bge::KeyCode::E)
  {
    m_Up = bge::Clamp(m_Up + 1.0f, -1.0f, 1.0f);
  }
  return false;
}
bool CameraControlSystem::OnMouseMoveEvent(bge::MouseMovedEvent& event)
{
  float xPos = event.GetX();
  float yPos = event.GetY();

  if (m_LastMouseX == 0.0f && m_LastMouseY == 0.0f)
  {
    m_LastMouseX = xPos;
    m_LastMouseY = yPos;
  }

  float deltaXMouse = 0.005f * (xPos - m_LastMouseX);
  float deltaYMouse = 0.005f * (yPos - m_LastMouseY);

  m_Yaw += deltaXMouse;
  m_Pitch += deltaYMouse;

  // BGE_INFO("[Yaw: {0}] [Pitch: {1}]", m_Yaw, m_Pitch);

  m_LastMouseX = xPos;
  m_LastMouseY = yPos;
  return false;
}