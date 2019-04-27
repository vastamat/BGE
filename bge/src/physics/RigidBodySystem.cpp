#include "physics/RigidBodySystem.h"

#include "logging/Log.h"

namespace bge
{

void RigidBodySystem::UpdateTransforms()
{
  for (size_t i = 0; i < m_BodyTransforms.size(); i++)
  {
    PhysicsDevice::GetBodyTransform(m_Entities[i], m_BodyTransforms[i]);
  }
}

void RigidBodySystem::AddBoxBodyComponent(Entity entity, float mass, float cx,
                                          float cy, float cz)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 0,
                  "Component already exists for this entity");

  PhysicsDevice::CreateBox(entity, mass, cx, cy, cz);

  m_Entities.push_back(entity);
  m_ColliderTypes.push_back(ColliderType::Box);

  m_BodyTransforms.emplace_back();
  m_BodyTransforms.back().SetScale(Vec3f(cx, cy, cz));

  m_EntityToComponentId[entity.GetId()] = m_Entities.size() - 1;
}

void RigidBodySystem::AddSphereBodyComponent(Entity entity, float mass,
                                             float radius)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 0,
                  "Component already exists for this entity");

  PhysicsDevice::CreateSphere(entity, mass, radius);

  m_Entities.push_back(entity);
  m_ColliderTypes.push_back(ColliderType::Sphere);

  m_BodyTransforms.emplace_back();
  m_BodyTransforms.back().SetScale(Vec3f(radius));

  m_EntityToComponentId[entity.GetId()] = m_Entities.size() - 1;
}

void RigidBodySystem::DestroyRigidBody(Entity entity)
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
    PhysicsDevice::DestroyBox(entityToRemove);
  }
  else
  {
    PhysicsDevice::DestroySphere(entityToRemove);
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

void RigidBodySystem::SetBodyPosition(Entity entity, const Vec3f& position)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");

  PhysicsDevice::SetBodyPosition(entity, position);
}

void RigidBodySystem::SetBodyVelocity(Entity entity, const Vec3f& velocity)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");

  PhysicsDevice::SetBodyVelocity(entity, velocity);
}
void RigidBodySystem::AddBodyVelocity(Entity entity, const Vec3f& amountToAdd)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");

  PhysicsDevice::AddBodyVelocity(entity, amountToAdd);
}

// uint32 RigidBodySystem::LookUpBody(Entity entity)
// {
//   BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
//                   "Component does not exist for this entity");
//   return m_Bodies[m_EntityToComponentId[entity.GetId()]];
// }

void RigidBodySystem::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<EntitiesDestroyedEvent>(
      BGE_BIND_EVENT_FN(RigidBodySystem::OnEntitiesDestroyed));
}

bool RigidBodySystem::OnEntitiesDestroyed(EntitiesDestroyedEvent& event)
{
  for (auto&& entity : event.GetEntities())
  {
    for (auto&& existingEntity : m_Entities)
    {
      if (entity == existingEntity)
      {
        DestroyRigidBody(entity);
      }
    }
  }

  return false;
}

} // namespace bge
