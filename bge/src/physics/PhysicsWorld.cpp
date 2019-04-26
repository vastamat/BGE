#include "physics/PhysicsWorld.h"

#include "events/PhysicsEvents.h"

namespace bge
{

void PhysicsWorld::SetEventCallback(const std::function<void(Event&)>& callback)
{
  m_EventCallback = callback;
}

void PhysicsWorld::Simulate()
{
  CollidedBodies collisions = PhysicsDevice::Simulate();

  m_RigidBodySystem.UpdateTransforms();

  EntitiesCollidedEvent event(collisions);
  m_EventCallback(event);
}

void PhysicsWorld::OnEvent(Event& event)
{
  m_RigidBodySystem.OnEvent(event);
  m_ColliderSystem.OnEvent(event);
}

} // namespace bge
