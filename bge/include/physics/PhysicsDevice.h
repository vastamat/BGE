#pragma once

#include "core/Common.h"
#include "ecs/Entity.h"
#include "math/Transform.h"

namespace bge
{

/**
 * Enum of the currently supported collider types
 */
enum ColliderType
{
  Box,
  Sphere
};

/**
 * Pair of bodies used for collision
 */
struct BodyPair
{
  uint16 m_BodyA{0};
  uint16 m_BodyB{0};
};

/**
 * Stores all collided bodies from a simulation step
 */
struct CollidedBodies
{
  BodyPair* m_Bodies{nullptr};
  uint32 m_Count{0};
};

namespace PhysicsDevice
{

/**
 * Initialize the physics device, which allocates all the buffers
 */
void Initialize();

/**
 * Step through the physics simulation
 */
CollidedBodies Simulate();

/**
 * Set the gravity value
 * @param gravity the new gravity value
 */
void SetGravity(float gravity);

/**
 * Create a box collider mapped to an entity
 * @param entity the entity which the collider is mapped to
 * @param position the location of the collider
 * @param rotation the orientation of the collider
 * @param size the size of the collider in each axis
 */
void MakeBoxCollider(Entity entity, const Vec3f& position,
                     const Quatf& rotation, const Vec3f& size);

/**
 * Create a sphere collider mapped to an entity
 * @param entity the entity which the collider is mapped to
 * @param position the location of the collider
 * @param radius the radius of the sphere
 */
void MakeSphereCollider(Entity entity, const Vec3f& position, float radius);

/**
 * Destroy a box collider mapped to an entity
 * @param entity the entity which the collider is mapped to
 */
void DestroyBoxCollider(Entity entity);

/**
 * Destroy a sphere collider mapped to an entity
 * @param entity the entity which the collider is mapped to
 */
void DestroySphereCollider(Entity entity);

// uint32 MakeBoxBody(uint32 entityId, float mass, float cx, float cy, float
// cz); uint32 MakeSphereBody(uint32 entityId, float mass, float radius);

/**
 * Create a box rigidbody mapped to an entity
 * @param entity the entity which the rigidbody is mapped to
 * @param mass the mass of the rigidbody
 * @param cx the width of the box
 * @param cy the height of the box
 * @param cz the depth of the box
 */
void CreateBox(Entity entity, float mass, float cx, float cy, float cz);

/**
 * Create a sphere rigidbody mapped to an entity
 * @param entity the entity which the rigidbody is mapped to
 * @param mass the mass of the rigidbody
 * @param radius the radius of the sphere
 */
void CreateSphere(Entity entity, float mass, float radius);

/**
 * Destroy a box rigidbody mapped to an entity
 * @param entity the entity which the box is mapped to
 */
void DestroyBox(Entity entity);

/**
 * Destroy a sphere rigidbody mapped to an entity
 * @param entity the entity which the sphere is mapped to
 */
void DestroySphere(Entity entity);

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
 * Set the position of a box collider
 * @param entity the entity which the collider is mapped to
 * @param position the new position
 */
void SetBoxColliderPosition(Entity entity, const Vec3f& position);

/**
 * Set the size of a box collider
 * @param entity the entity which the collider is mapped to
 * @param size the new size
 */
void SetBoxColliderSize(Entity entity, const Vec3f& size);
// void SetBoxColliderBody(uint32 colliderId, uint32 bodyId);

/**
 * Set the position of a sphere collider
 * @param entity the entity which the collider is mapped to
 * @param position the new position
 */
void SetSphereColliderPosition(Entity entity, const Vec3f& position);

/**
 * Set the radius of a sphere collider
 * @param entity the entity which the collider is mapped to
 * @param radius the new radius
 */
void SetSphereColliderRadius(Entity entity, float radius);
// void SetSphereColliderBody(uint32 colliderId, uint32 bodyId);

/**
 * Get the transform of an entity's physics body
 * @param entity the entity which the body is mapped to
 * @param output the transform into which to write the output
 */
void GetBodyTransform(Entity entity, Transform& output);

/**
 * Get the box collider scale of an entity
 * @param entity the entity which the collider is mapped to
 * @return the size of the collider in each axis
 */
Vec3f GetBoxColliderScale(Entity entity);

/**
 * Get the sphere collider radius of an entity
 * @param entity the entity which the collider is mapped to
 * @return the radius of the sphere
 */
float GetSphereColliderRadius(Entity entity);

/**
 * @return get all allocated box collider transforms
 */
std::vector<Mat4f> GetAllBoxColliderTransforms();

/**
 * @return get all allocated sphere collider transforms
 */
std::vector<Mat4f> GetAllSphereColliderTransforms();

} // namespace PhysicsDevice
} // namespace bge
