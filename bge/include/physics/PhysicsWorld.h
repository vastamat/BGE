#pragma once

#include "RigidBodySystem.h"

#include <functional>
#include <vector>

namespace bge
{

class PhysicsWorld
{
public:
  void SetEventCallback(const std::function<void(Event&)>& callback);

  void Simulate();

  FORCEINLINE RigidBodySystem& GetRigidBodySystem()
  {
    return m_RigidBodySystem;
  }

  void OnEvent(Event& event);

private:
  RigidBodySystem m_RigidBodySystem;

  // Event Callback used to fire events to the app
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge
