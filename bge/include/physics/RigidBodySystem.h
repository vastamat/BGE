#pragma once

#include "PhysicsDevice.h"

#include "ecs/Entity.h"
#include "events/ECSEvents.h"
#include "math/Transform.h"

#include <unordered_map>

namespace bge
{

/**
 * System which handles rigid bodies
 */
class RigidBodySystem
{
public:
  /**
   * Updates the array of transforms to match the physics world
   */
  void UpdateTransforms();

  /**
   * Add a box rigidbody mapped to an entity
   * @param entity the entity which the rigidbody is mapped to
   * @param mass the mass of the rigidbody
   * @param cx the width of the box
   * @param cy the height of the box
   * @param cz the depth of the box
   */
  void AddBoxBodyComponent(Entity entity, float mass, float cx, float cy,
                           float cz);

  /**
   * Add a sphere rigidbody mapped to an entity
   * @param entity the entity which the rigidbody is mapped to
   * @param mass the mass of the rigidbody
   * @param radius the radius of the sphere
   */
  void AddSphereBodyComponent(Entity entity, float mass, float radius);

  /**
   * Destroy an existing rigidbody mapped to an entity
   * @param entity the entity which the rigidbody is mapped to
   */
  void DestroyRigidBody(Entity entity);

  /**
   * Set the position of a physics body
   * @param entity the entity which the body is mapped to
   * @param position the new position
   */
  void SetBodyPosition(Entity entity, const Vec3f& position);

  /**
   * Set the velocity of a physics body
   * @param entity the entity which the body is mapped to
   * @param velocity the new velocity
   */
  void SetBodyVelocity(Entity entity, const Vec3f& velocity);

  /**
   * Add to the velocity of a physics body
   * @param entity the entity which the body is mapped to
   * @param amountToAdd the amout of velocity to add
   */
  void AddBodyVelocity(Entity entity, const Vec3f& amountToAdd);

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
  // Vector of all entities which have a rigidbody
  std::vector<Entity> m_Entities;
  // Vector of the collider types for every body allocated
  std::vector<ColliderType> m_ColliderTypes;
  // Vector of the transforms of each collider
  std::vector<Transform> m_BodyTransforms;
};

} // namespace bge
