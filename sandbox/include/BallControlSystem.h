#pragma once

#include <ecs/GameWorld.h>
#include <events/KeyEvents.h>
#include <events/MouseEvents.h>

class BallControlSystem : public bge::GameSystem
{
public:
  explicit BallControlSystem(bge::Entity controlledEntity, float speed);

  virtual void Tick(float deltaSeconds) override;
  virtual void OnEvent(bge::Event& event) override;

private:
  bool OnKeyPressEvent(bge::KeyPressedEvent& event);

  bge::Entity m_ControlledEntity;
  float m_Speed;
};