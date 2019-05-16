#pragma once

#include "PhysicsDevice.h"

#include "ecs/Entity.h"
#include "events/ECSEvents.h"
#include "math/Transform.h"

#include <unordered_map>

namespace bge
{

/**
 * System which handles static colliders
 */
class ColliderSystem
{
public:
  /**
   * Adds a new box collider mapped to an entity
   * @param entity the entity which the collider will be mapped to
   * @param position the position of the collider in space
   * @param rotation the orientation of the collider
   * @param size the size of the collider in each axis
   */
  void AddBoxCollider(Entity entity, const Vec3f& position,
                      const Quatf& rotation, const Vec3f& size);

  /**
   * Adds a new sphere collider mapped to an entity
   * @param entity the entity which the collider will be mapped to
   * @param position the position of the collider in space
   * @param radius the radius of the sphere
   */
  void AddSphereCollider(Entity entity, const Vec3f& position, float radius);

  /**
   * Destroy an existing collider mapped to an entity
   * @param entity the entity which the collider is mapped to
   */
  void DestroyCollider(Entity entity);

  /**
   * Getter for all the transforms of the allocated colliders
   * @return the vector of transforms
   */
  FORCEINLINE const std::vector<Transform>& GetBodyTransforms() const
  {
    return m_BodyTransforms;
  }

  /**
   * Event handler function
   * @param event the propagated event
   */
  void OnEvent(Event& event);

private:
  /**
   * Handler of destroyed entities event
   * @param event the propagated event
   */
  bool OnEntitiesDestroyed(EntitiesDestroyedEvent& event);

private:
  // Map of entity's Id to the array index of the vectors
  std::unordered_map<uint32, uint32> m_EntityToComponentId;
  // Vector of all entities which have a collider
  std::vector<Entity> m_Entities;
  // Vector of the collider types for every collider allocated
  std::vector<ColliderType> m_ColliderTypes;
  // Vector of the transforms of each collider
  std::vector<Transform> m_BodyTransforms;
};

} // namespace bge