#pragma once

#include "ColliderSystem.h"
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
  FORCEINLINE ColliderSystem& GetColliderSystem() { return m_ColliderSystem; }

  void OnEvent(Event& event);

private:
  RigidBodySystem m_RigidBodySystem;
  ColliderSystem m_ColliderSystem;

  // Event Callback used to fire events to the app
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge
