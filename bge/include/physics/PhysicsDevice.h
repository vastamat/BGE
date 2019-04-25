#pragma once

#include "core/Common.h"
#include "math/Transform.h"

namespace bge
{

struct RigidBody
{
  uint32 m_ColliderId;
  uint32 m_BodyId;
};

struct BodyPair
{
  uint16 m_BodyA;
  uint16 m_BodyB;
};

struct CollidedBodies
{
  BodyPair* m_Bodies;
  uint32 m_Count;
};

namespace PhysicsDevice
{

void Initialize();
CollidedBodies Simulate();

uint32 MakeBoxCollider(float position[3], float rotation[4], float size[3]);
uint32 MakeSphereCollider(float position[3], float radius);
uint32 MakeBoxBody(float mass, float cx, float cy, float cz);
uint32 MakeSphereBody(float mass, float radius);

RigidBody CreateBox(float mass, float cx, float cy, float cz);
RigidBody CreateSphere(float mass, float radius);

void SetBodyPosition(uint32 bodyId, float position[3]);

void SetBoxColliderPosition(uint32 colliderId, float position[3]);
void SetBoxColliderSize(uint32 colliderId, float size[3]);
void SetBoxColliderBody(uint32 colliderId, uint32 bodyId);

void SetSphereColliderPosition(uint32 colliderId, float position[3]);
void SetSphereColliderRadius(uint32 colliderId, float radius);
void SetSphereColliderBody(uint32 colliderId, uint32 bodyId);

void GetBodyTransform(uint32 bodyId, Transform& output);
Vec3f GetBoxColliderScale(uint32 boxColliderId);
float GetSphereColliderRadius(uint32 sphereColliderId);

} // namespace PhysicsDevice
} // namespace bge
