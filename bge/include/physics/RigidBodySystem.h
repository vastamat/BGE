#pragma once

#include "PhysicsDevice.h"

#include "ecs/Entity.h"
#include "events/ECSEvents.h"
#include "math/Transform.h"

#include <unordered_map>

namespace bge
{

class RigidBodySystem
{
public:
  void UpdateTransforms();

  void AddBoxBodyComponent(Entity entity, float mass, float cx, float cy,
                           float cz);
  void AddSphereBodyComponent(Entity entity, float mass, float radius);

  void DestroyRigidBody(Entity entity);
  uint32 LookUpBody(Entity entity);

  FORCEINLINE const std::vector<Transform>& GetBodyTransforms() const
  {
    return m_BodyTransforms;
  }

  void OnEvent(Event& event);

private:
  bool OnEntitiesDestroyed(EntitiesDestroyedEvent& event);

private:
  std::unordered_map<uint32, uint32> m_EntityToComponentId;
  std::vector<uint32> m_Bodies;
  std::vector<Transform> m_BodyTransforms;
  std::vector<Entity> m_Entities;
};

} // namespace bge
