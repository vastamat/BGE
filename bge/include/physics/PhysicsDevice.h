#pragma once

#include "core/Common.h"
#include "ecs/Entity.h"
#include "math/Transform.h"

namespace bge
{

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

// TODO: Saved for later
// uint32 MakeBoxCollider(uint32 entityId, float position[3], float rotation[4],
//                        float size[3]);
// uint32 MakeSphereCollider(uint32 entityId, float position[3], float radius);
// uint32 MakeBoxBody(uint32 entityId, float mass, float cx, float cy, float
// cz); uint32 MakeSphereBody(uint32 entityId, float mass, float radius);

void CreateBox(Entity entity, float mass, float cx, float cy, float cz);
void CreateSphere(Entity entity, float mass, float radius);

void DestroyBox(Entity entity);
void DestroySphere(Entity entity);

// void SetBodyPosition(uint32 bodyId, float position[3]);

// void SetBoxColliderPosition(uint32 colliderId, float position[3]);
// void SetBoxColliderSize(uint32 colliderId, float size[3]);
// void SetBoxColliderBody(uint32 colliderId, uint32 bodyId);

// void SetSphereColliderPosition(uint32 colliderId, float position[3]);
// void SetSphereColliderRadius(uint32 colliderId, float radius);
// void SetSphereColliderBody(uint32 colliderId, uint32 bodyId);

void GetBodyTransform(Entity entity, Transform& output);
Vec3f GetBoxColliderScale(Entity entity);
float GetSphereColliderRadius(Entity entity);

} // namespace PhysicsDevice
} // namespace bge
