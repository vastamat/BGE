#pragma once

#include "ColliderSystem.h"
#include "RigidBodySystem.h"

#include <functional>
#include <vector>

namespace bge
{

/**
 * The physics world which encompasses the whole physics module of the framework
 */
class PhysicsWorld
{
public:
  /**
   * Sets the internal event callback variable
   * It's used to broadcast events to the application layer
   * @param callback the function pointer to use to broadcast events
   */
  void SetEventCallback(const std::function<void(Event&)>& callback);

  /**
   * Steps the physics simulation
   */
  void Simulate();

  FORCEINLINE RigidBodySystem& GetRigidBodySystem()
  {
    return m_RigidBodySystem;
  }
  FORCEINLINE ColliderSystem& GetColliderSystem() { return m_ColliderSystem; }

  /**
   * Calls the OnEvent function of all systems
   * @param event the broadcast event
   */
  void OnEvent(Event& event);

private:
  RigidBodySystem m_RigidBodySystem;
  ColliderSystem m_ColliderSystem;

  // Event Callback used to fire events to the app
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge
