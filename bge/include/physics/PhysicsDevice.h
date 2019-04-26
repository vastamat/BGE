#pragma once

#include "core/Common.h"
#include "ecs/Entity.h"
#include "math/Transform.h"

namespace bge
{

enum ColliderType
{
  Box,
  Sphere
};

struct BodyPair
{
  uint16 m_BodyA{0};
  uint16 m_BodyB{0};
};

struct CollidedBodies
{
  BodyPair* m_Bodies{nullptr};
  uint32 m_Count{0};
};

namespace PhysicsDevice
{

void Initialize();
CollidedBodies Simulate();
void SetGravity(float gravity);

// TODO: Saved for later
void MakeBoxCollider(Entity entity, const Vec3f& position,
                     const Quatf& rotation, const Vec3f& size);
void MakeSphereCollider(Entity entity, const Vec3f& position, float radius);

void DestroyBoxCollider(Entity entity);
void DestroySphereCollider(Entity entity);

// uint32 MakeBoxBody(uint32 entityId, float mass, float cx, float cy, float
// cz); uint32 MakeSphereBody(uint32 entityId, float mass, float radius);

void CreateBox(Entity entity, float mass, float cx, float cy, float cz);
void CreateSphere(Entity entity, float mass, float radius);

void DestroyBox(Entity entity);
void DestroySphere(Entity entity);

void SetBodyPosition(Entity entity, const Vec3f& position);

void SetBoxColliderPosition(Entity entity, const Vec3f& position);
void SetBoxColliderSize(Entity entity, const Vec3f& size);
// void SetBoxColliderBody(uint32 colliderId, uint32 bodyId);

void SetSphereColliderPosition(Entity entity, const Vec3f& position);
void SetSphereColliderRadius(Entity entity, float radius);
// void SetSphereColliderBody(uint32 colliderId, uint32 bodyId);

void GetBodyTransform(Entity entity, Transform& output);
Vec3f GetBoxColliderScale(Entity entity);
float GetSphereColliderRadius(Entity entity);

} // namespace PhysicsDevice
} // namespace bge
