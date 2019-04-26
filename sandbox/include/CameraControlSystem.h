#pragma once

#include <ecs/GameWorld.h>
#include <events/KeyEvents.h>
#include <events/MouseEvents.h>
#include <math/Vec.h>

class CameraControlSystem : public bge::GameSystem
{
public:
  explicit CameraControlSystem(uint32 trackedCameraId);

  virtual void Tick(float deltaSeconds) override;
  virtual void OnEvent(bge::Event& event) override;

private:
  bool OnKeyPressEvent(bge::KeyPressedEvent& event);
  bool OnKeyReleaseEvent(bge::KeyReleasedEvent& event);
  bool OnMouseMoveEvent(bge::MouseMovedEvent& event);

  uint32 m_TrackedCameraId = 0;

  bge::Vec3f m_Position{0.0f};

  float m_Forward = 0.0f;
  float m_Right = 0.0f;
  float m_Up = 0.0f;

  float m_LastMouseX = 0.0f;
  float m_LastMouseY = 0.0f;

  float m_Pitch = 0.0f;
  float m_Yaw = 0.0f;
};