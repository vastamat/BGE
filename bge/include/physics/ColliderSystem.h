#pragma once

#include "PhysicsDevice.h"

#include "ecs/Entity.h"
#include "events/ECSEvents.h"
#include "math/Transform.h"

#include <unordered_map>

namespace bge
{

class ColliderSystem
{
public:
  void AddBoxCollider(Entity entity, const Vec3f& position,
                      const Quatf& rotation, const Vec3f& size);
  void AddSphereCollider(Entity entity, const Vec3f& position, float radius);

  void DestroyCollider(Entity entity);

  FORCEINLINE const std::vector<Transform>& GetBodyTransforms() const
  {
    return m_BodyTransforms;
  }

  void OnEvent(Event& event);

private:
  bool OnEntitiesDestroyed(EntitiesDestroyedEvent& event);

private:
  std::unordered_map<uint32, uint32> m_EntityToComponentId;
  std::vector<Entity> m_Entities;
  std::vector<ColliderType> m_ColliderTypes;
  std::vector<Transform> m_BodyTransforms;
};

} // namespace bge