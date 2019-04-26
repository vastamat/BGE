#include "CameraControlSystem.h"

#include <core/Application.h>
#include <input/Input.h>
#include <math/Quat.h>

CameraControlSystem::CameraControlSystem(uint32 trackedCameraId)
    : m_TrackedCameraId(trackedCameraId)
{
}

void CameraControlSystem::Tick(float deltaSeconds)
{
  auto mousePos = bge::Input::GetMousePosition();

  if (m_LastMouseX == 0.0f && m_LastMouseY == 0.0f)
  {
    m_LastMouseX = mousePos.first;
    m_LastMouseY = mousePos.second;
  }

  float deltaXMouse = 0.005f * (mousePos.first - m_LastMouseX);
  float deltaYMouse = 0.005f * (mousePos.second - m_LastMouseY);

  m_Yaw += deltaXMouse;
  m_Pitch += deltaYMouse;

  // BGE_INFO("[Yaw: {0}] [Pitch: {1}]", m_Yaw, m_Pitch);

  m_LastMouseX = mousePos.first;
  m_LastMouseY = mousePos.second;

  bge::Quatf orientation =
      (bge::Quatf::GenRotationX(m_Pitch) * bge::Quatf::GenRotationY(m_Yaw)) *
      0.05f;

  if (bge::Input::IsKeyPressed(bge::KeyCode::W))
  {
    m_Forward += 50.0f;
  }
  if (bge::Input::IsKeyPressed(bge::KeyCode::S))
  {
    m_Forward -= 50.0f;
  }

  if (bge::Input::IsKeyPressed(bge::KeyCode::A))
  {
    m_Right -= 50.0f;
  }
  if (bge::Input::IsKeyPressed(bge::KeyCode::D))
  {
    m_Right += 50.0f;
  }

  if (bge::Input::IsKeyPressed(bge::KeyCode::Q))
  {
    m_Up += 1.0f;
  }
  if (bge::Input::IsKeyPressed(bge::KeyCode::E))
  {
    m_Up -= 1.0f;
  }

  bge::Quatf conjugated = orientation.GetConjugated();

  bge::Vec3f zAxis =
      bge::Quatf::RotateVec(conjugated, bge::Vec3f(0.0f, 0.0f, -1.0f));
  bge::Vec3f xAxis =
      bge::Quatf::RotateVec(conjugated, bge::Vec3f(1.0f, 0.0f, 0.0f));
  bge::Vec3f yAxis(0.0f, 1.0f, 0.0f);

  bge::Vec3f position =
      (zAxis * m_Forward) + (xAxis * m_Right) + (yAxis * m_Up * 0.1f);

  bge::Mat4f view =
      orientation.ToMat4() * bge::GenTranslationMat(position * -1);

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

bool CameraControlSystem::OnKeyPressEvent(bge::KeyPressedEvent& event) {}
bool CameraControlSystem::OnKeyReleaseEvent(bge::KeyReleasedEvent& event) {}
bool CameraControlSystem::OnMouseMoveEvent(bge::MouseMovedEvent& event) {}