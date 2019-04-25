#include "physics/RigidBodySystem.h"

#include "logging/Log.h"

namespace bge
{

void RigidBodySystem::UpdateTransforms()
{
  for (size_t i = 0; i < m_BodyTransforms.size(); i++)
  {
    PhysicsDevice::GetBodyTransform(m_Bodies[i], m_BodyTransforms[i]);
  }
}

void RigidBodySystem::AddBoxBodyComponent(Entity entity, float mass, float cx,
                                          float cy, float cz)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 0,
                  "Component already exists for this entity");

  RigidBody body = PhysicsDevice::CreateBox(mass, cx, cy, cz);

  m_Entities.push_back(entity);
  m_Bodies.push_back(body.m_BodyId);

  m_BodyTransforms.emplace_back();
  m_BodyTransforms.back().SetScale(
      PhysicsDevice::GetBoxColliderScale(body.m_ColliderId));

  m_EntityToComponentId[entity.GetId()] = m_Bodies.size() - 1;
}

void RigidBodySystem::AddSphereBodyComponent(Entity entity, float mass,
                                             float radius)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 0,
                  "Component already exists for this entity");

  RigidBody body = PhysicsDevice::CreateSphere(mass, radius);

  m_Entities.push_back(entity);
  m_Bodies.push_back(body.m_BodyId);

  m_BodyTransforms.emplace_back();
  m_BodyTransforms.back().SetScale(
      Vec3f(PhysicsDevice::GetSphereColliderRadius(body.m_ColliderId)));

  m_EntityToComponentId[entity.GetId()] = m_Bodies.size() - 1;
}

void RigidBodySystem::DestroyRigidBody(Entity entity)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");

  uint32 idToRemove = m_EntityToComponentId[entity.GetId()];
  uint32 lastComponentIndex = m_Bodies.size() - 1;
  Entity lastEntity = m_Entities[lastComponentIndex];

  m_Bodies[idToRemove] = m_Bodies[lastComponentIndex];
  m_Entities[idToRemove] = m_Entities[lastComponentIndex];
  m_BodyTransforms[idToRemove] = m_BodyTransforms[lastComponentIndex];

  m_Bodies.pop_back();
  m_Entities.pop_back();
  m_BodyTransforms.pop_back();

  m_EntityToComponentId[lastEntity.GetId()] = idToRemove;
  m_EntityToComponentId.erase(entity.GetId());
}

uint32 RigidBodySystem::LookUpBody(Entity entity)
{
  BGE_CORE_ASSERT(m_EntityToComponentId.count(entity.GetId()) == 1,
                  "Component does not exist for this entity");
  return m_Bodies[m_EntityToComponentId[entity.GetId()]];
}

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
