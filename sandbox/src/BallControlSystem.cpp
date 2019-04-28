#include "BallControlSystem.h"

#include <core/Application.h>
#include <input/Input.h>

BallControlSystem::BallControlSystem(bge::Entity controlledEntity, float speed)
    : m_ControlledEntity(controlledEntity)
    , m_Speed(speed)
{
}

void BallControlSystem::Tick(float deltaSeconds)
{
  bge::RigidBodySystem& rigidBodySystem =
      bge::Application::Get().GetWorld().GetPhysicsWorld().GetRigidBodySystem();

  if (bge::Input::IsKeyPressed(bge::KeyCode::ArrowUp))
  {
    rigidBodySystem.AddBodyVelocity(
        m_ControlledEntity, bge::Vec3f(0.0f, 0.0f, -m_Speed * deltaSeconds));
  }
  if (bge::Input::IsKeyPressed(bge::KeyCode::ArrowDown))
  {
    rigidBodySystem.AddBodyVelocity(
        m_ControlledEntity, bge::Vec3f(0.0f, 0.0f, m_Speed * deltaSeconds));
  }

  if (bge::Input::IsKeyPressed(bge::KeyCode::ArrowLeft))
  {
    rigidBodySystem.AddBodyVelocity(
        m_ControlledEntity, bge::Vec3f(-m_Speed * deltaSeconds, 0.0f, 0.0f));
  }
  if (bge::Input::IsKeyPressed(bge::KeyCode::ArrowRight))
  {
    rigidBodySystem.AddBodyVelocity(
        m_ControlledEntity, bge::Vec3f(m_Speed * deltaSeconds, 0.0f, 0.0f));
  }

  if (bge::Input::IsKeyPressed(bge::KeyCode::Space))
  {
    rigidBodySystem.AddBodyVelocity(
        m_ControlledEntity, bge::Vec3f(0.0f, m_Speed * deltaSeconds, 0.0f));
  }
}
void BallControlSystem::OnEvent(bge::Event& event)
{
  // bge::EventDispatcher dispatcher(event);

  // dispatcher.Dispatch<bge::KeyPressedEvent>(
  //     BGE_BIND_EVENT_FN(BallControlSystem::OnKeyPressEvent));
}

bool BallControlSystem::OnKeyPressEvent(bge::KeyPressedEvent& event)
{
  // bge::KeyCode key = event.GetKeyCode();

  // bge::RigidBodySystem& rigidBodySystem =
  //     bge::Application::Get().GetWorld().GetPhysicsWorld().GetRigidBodySystem();

  // if (key == bge::KeyCode::ArrowUp)
  // {
  //   rigidBodySystem.AddBodyVelocity(m_ControlledEntity,
  //                                   bge::Vec3f(0.0f, 0.0f, -m_Speed));
  // }
  // if (key == bge::KeyCode::ArrowDown)
  // {
  //   rigidBodySystem.AddBodyVelocity(m_ControlledEntity,
  //                                   bge::Vec3f(0.0f, 0.0f, m_Speed));
  // }

  // if (key == bge::KeyCode::ArrowLeft)
  // {
  //   rigidBodySystem.AddBodyVelocity(m_ControlledEntity,
  //                                   bge::Vec3f(-m_Speed, 0.0f, 0.0f));
  // }
  // if (key == bge::KeyCode::ArrowRight)
  // {
  //   rigidBodySystem.AddBodyVelocity(m_ControlledEntity,
  //                                   bge::Vec3f(m_Speed, 0.0f, 0.0f));
  // }

  // if (key == bge::KeyCode::Space)
  // {
  //   rigidBodySystem.AddBodyVelocity(m_ControlledEntity,
  //                                   bge::Vec3f(0.0f, m_Speed, 0.0f));
  // }

  return false;
}