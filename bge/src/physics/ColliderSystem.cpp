#include "physics/ColliderSystem.h"

#include "logging/Log.h"

namespace bge
{

void ColliderSystem::AddBoxCollider(Entity entity, const Vec3f& position,
                                    const Quatf& rotation, const Vec3f& size)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 0,
                  "Component already exists for this entity");

  PhysicsDevice::MakeBoxCollider(entity, position, rotation, size);

  m_Entities.push_back(entity);
  m_ColliderTypes.push_back(ColliderType::Box);

  m_BodyTransforms.emplace_back(position, size, rotation);

  m_EntityToComponentId[entity.GetId()] = m_Entities.size() - 1;
}

void ColliderSystem::AddSphereCollider(Entity entity, const Vec3f& position,
                                       float radius)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 0,
                  "Component already exists for this entity");

  PhysicsDevice::MakeSphereCollider(entity, position, radius);

  m_Entities.push_back(entity);
  m_ColliderTypes.push_back(ColliderType::Sphere);

  m_BodyTransforms.emplace_back(position, Vec3f(radius), Quatf());

  m_EntityToComponentId[entity.GetId()] = m_Entities.size() - 1;
}

void ColliderSystem::DestroyCollider(Entity entity)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");

  uint32 idToRemove = m_EntityToComponentId[entity.GetId()];
  uint32 lastComponentIndex = m_Entities.size() - 1;
  Entity lastEntity = m_Entities[lastComponentIndex];

  Entity entityToRemove = m_Entities[idToRemove];
  ColliderType type = m_ColliderTypes[idToRemove];

  if (type == ColliderType::Box)
  {
    PhysicsDevice::DestroyBoxCollider(entityToRemove);
  }
  else
  {
    PhysicsDevice::DestroySphereCollider(entityToRemove);
  }

  m_Entities[idToRemove] = m_Entities[lastComponentIndex];
  m_BodyTransforms[idToRemove] = m_BodyTransforms[lastComponentIndex];
  m_ColliderTypes[idToRemove] = m_ColliderTypes[lastComponentIndex];

  m_Entities.pop_back();
  m_BodyTransforms.pop_back();
  m_ColliderTypes.pop_back();

  m_EntityToComponentId[lastEntity.GetId()] = idToRemove;
  m_EntityToComponentId.erase(entity.GetId());
}

void ColliderSystem::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<EntitiesDestroyedEvent>(
      BGE_BIND_EVENT_FN(ColliderSystem::OnEntitiesDestroyed));
}

bool ColliderSystem::OnEntitiesDestroyed(EntitiesDestroyedEvent& event)
{
  for (auto&& entity : event.GetEntities())
  {
    for (auto&& existingEntity : m_Entities)
    {
      if (entity == existingEntity)
      {
        DestroyCollider(entity);
      }
    }
  }

  return false;
}

} // namespace bge
