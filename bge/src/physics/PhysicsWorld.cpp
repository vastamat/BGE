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

RigidBody PhysicsWorld::CreateBox(float mass, float cx, float cy, float cz)
{
  return PhysicsDevice::CreateBox(mass, cx, cy, cz);
}

RigidBody PhysicsWorld::CreateSphere(float mass, float radius)
{
  return PhysicsDevice::CreateSphere(mass, radius);
}

void PhysicsWorld::OnEvent(Event& event) { m_RigidBodySystem.OnEvent(event); }

} // namespace bge
