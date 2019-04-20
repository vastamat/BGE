#include "physics/PhysicsWorld.h"

namespace bge
{

void PhysicsWorld::Simulate()
{
  CollidedBodies collisions = PhysicsDevice::Simulate();

  for (size_t i = 0; i < collisions.m_Count; i++)
  {
    for (auto&& handler : m_OnCollisionEvents)
    {
      handler(collisions.m_Bodies[i].m_BodyA, collisions.m_Bodies[i].m_BodyB);
    }
  }
}

Box PhysicsWorld::CreateBox(float mass, float cx, float cy, float cz)
{
  return PhysicsDevice::CreateBox(mass, cx, cy, cz);
}

Sphere PhysicsWorld::CreateSphere(float mass, float radius)
{
  return PhysicsDevice::CreateSphere(mass, radius);
}

void PhysicsWorld::AddOnCollideEventHandler(const OnCollideCallbackFn& handler)
{
  m_OnCollisionEvents.push_back(handler);
}

template <> BoxSystem* PhysicsWorld::GetComponentSystem<BoxSystem>()
{
  return &m_BoxSystem;
}

template <> SphereSystem* PhysicsWorld::GetComponentSystem<SphereSystem>()
{
  return &m_SphereSystem;
}

} // namespace bge
