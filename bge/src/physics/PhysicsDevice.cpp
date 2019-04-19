#include "physics/PhysicsDevice.h"

#include "logging/Log.h"

#include <nudge/nudge.h>

#include <immintrin.h>

namespace bge
{

static const uint32 s_maxBodyCount = 2048;
static const uint32 s_maxBoxCount = 2048;
static const uint32 s_maxSphereCount = 2048;
static const uint32 s_steps = 2;
static const uint32 s_iterations = 20;
static const float s_timeStep = 1.0f / (60.0f * (float)s_steps);
static const float s_damping = 1.0f - s_timeStep * 0.25f;

static nudge::Arena s_arena;
static nudge::BodyData s_bodies;
static nudge::ColliderData s_colliders;
static nudge::ContactData s_contactData;
static nudge::ContactCache s_contactCache;
static nudge::ActiveBodies s_activeBodies;

static const nudge::Transform s_identityTransform = {
    {}, 0, {0.0f, 0.0f, 0.0f, 1.0f}};

namespace PhysicsDevice
{

void Initialize()
{
  // Print information about instruction set.
#ifdef __AVX2__
  BGE_CORE_INFO("Using 8-wide AVX");
#else
  BGE_CORE_INFO("Using 4-wide SSE");
#if defined(__SSE4_1__) || defined(__AVX__)
  BGE_CORE_INFO("BLENDVPS: Enabled");
#else
  BGE_CORE_INFO("BLENDVPS: Disabled");
#endif
#endif

#ifdef __FMA__
  BGE_CORE_INFO("FMA: Enabled");
#else
  BGE_CORE_INFO("FMA: Disabled");
#endif

  // Allocate memory for simulation s_arena.
  s_arena.size = 64 * 1024 * 1024; // 64 MegaBytes
  s_arena.data = _mm_malloc(s_arena.size, 4096);

  // Allocate memory for s_bodies, s_colliders, and contacts.
  s_activeBodies.capacity = s_maxBoxCount;
  s_activeBodies.indices =
      static_cast<uint16_t*>(_mm_malloc(sizeof(uint16_t) * s_maxBodyCount, 64));

  s_bodies.idle_counters =
      static_cast<uint8_t*>(_mm_malloc(sizeof(uint8_t) * s_maxBodyCount, 64));
  s_bodies.transforms = static_cast<nudge::Transform*>(
      _mm_malloc(sizeof(nudge::Transform) * s_maxBodyCount, 64));
  s_bodies.momentum = static_cast<nudge::BodyMomentum*>(
      _mm_malloc(sizeof(nudge::BodyMomentum) * s_maxBodyCount, 64));
  s_bodies.properties = static_cast<nudge::BodyProperties*>(
      _mm_malloc(sizeof(nudge::BodyProperties) * s_maxBodyCount, 64));

  s_colliders.boxes.data = static_cast<nudge::BoxCollider*>(
      _mm_malloc(sizeof(nudge::BoxCollider) * s_maxBoxCount, 64));
  s_colliders.boxes.tags =
      static_cast<uint16_t*>(_mm_malloc(sizeof(uint16_t) * s_maxBoxCount, 64));
  s_colliders.boxes.transforms = static_cast<nudge::Transform*>(
      _mm_malloc(sizeof(nudge::Transform) * s_maxBoxCount, 64));

  s_colliders.spheres.data = static_cast<nudge::SphereCollider*>(
      _mm_malloc(sizeof(nudge::SphereCollider) * s_maxSphereCount, 64));
  s_colliders.spheres.tags = static_cast<uint16_t*>(
      _mm_malloc(sizeof(uint16_t) * s_maxSphereCount, 64));
  s_colliders.spheres.transforms = static_cast<nudge::Transform*>(
      _mm_malloc(sizeof(nudge::Transform) * s_maxSphereCount, 64));

  s_contactData.capacity = s_maxBodyCount * 64;
  s_contactData.bodies = static_cast<nudge::BodyPair*>(
      _mm_malloc(sizeof(nudge::BodyPair) * s_contactData.capacity, 64));
  s_contactData.data = static_cast<nudge::Contact*>(
      _mm_malloc(sizeof(nudge::Contact) * s_contactData.capacity, 64));
  s_contactData.tags = static_cast<uint64_t*>(
      _mm_malloc(sizeof(uint64_t) * s_contactData.capacity, 64));
  s_contactData.sleeping_pairs = static_cast<uint32_t*>(
      _mm_malloc(sizeof(uint32_t) * s_contactData.capacity, 64));

  s_contactCache.capacity = s_maxBodyCount * 64;
  s_contactCache.data = static_cast<nudge::CachedContactImpulse*>(_mm_malloc(
      sizeof(nudge::CachedContactImpulse) * s_contactCache.capacity, 64));
  s_contactCache.tags = static_cast<uint64_t*>(
      _mm_malloc(sizeof(uint64_t) * s_contactCache.capacity, 64));

  // The first body is the static world.
  s_bodies.count = 1;
  s_bodies.idle_counters[0] = 0;
  s_bodies.transforms[0] = s_identityTransform;
  memset(s_bodies.momentum, 0, sizeof(s_bodies.momentum[0]));
  memset(s_bodies.properties, 0, sizeof(s_bodies.properties[0]));
}

CollidedBodies Simulate()
{
  for (uint32 n = 0; n < s_steps; ++n)
  {
    // Setup a temporary memory s_arena. The same temporary memory is reused
    // each iteration.
    nudge::Arena temporary = s_arena;

    // Find contacts.
    // NOTE: Custom constraints should be added as body connections.
    nudge::BodyConnections connections = {};

    nudge::collide(&s_activeBodies, &s_contactData, s_bodies, s_colliders,
                   connections, temporary);

    CollidedBodies collidedBodies;
    collidedBodies.m_Count = s_contactData.count;
    memcpy(collidedBodies.m_Bodies, s_contactData.bodies,
           s_contactData.count * sizeof(s_contactData.bodies[0]));

    // NOTE: Custom contacts can be added here, e.g., against the static
    // environment.

    // Apply gravity and damping.
    for (uint32 i = 0; i < s_activeBodies.count; ++i)
    {
      uint32 index = s_activeBodies.indices[i];

      s_bodies.momentum[index].velocity[1] -= 9.82f * s_timeStep;

      s_bodies.momentum[index].velocity[0] *= s_damping;
      s_bodies.momentum[index].velocity[1] *= s_damping;
      s_bodies.momentum[index].velocity[2] *= s_damping;

      s_bodies.momentum[index].angular_velocity[0] *= s_damping;
      s_bodies.momentum[index].angular_velocity[1] *= s_damping;
      s_bodies.momentum[index].angular_velocity[2] *= s_damping;
    }

    // Read previous impulses from contact cache.
    nudge::ContactImpulseData* contactImpulses =
        nudge::read_cached_impulses(s_contactCache, s_contactData, &temporary);

    // Setup contact constraints and apply the initial impulses.
    nudge::ContactConstraintData* contactConstraints =
        nudge::setup_contact_constraints(s_activeBodies, s_contactData,
                                         s_bodies, contactImpulses, &temporary);

    // Apply contact impulses. Increasing the number of iterations will improve
    // stability.
    for (uint32 i = 0; i < s_iterations; ++i)
    {
      nudge::apply_impulses(contactConstraints, s_bodies);
      // NOTE: Custom constraint impulses should be applied here.
    }

    // Update contact impulses.
    nudge::update_cached_impulses(contactConstraints, contactImpulses);

    // Write the updated contact impulses to the cache.
    nudge::write_cached_impulses(&s_contactCache, s_contactData,
                                 contactImpulses);

    // Move active s_bodies.
    nudge::advance(s_activeBodies, s_bodies, s_timeStep);

    // return collided bodies
    return collidedBodies;
  }
}

uint32 MakeBoxCollider(float position[3], float rotation[4], float size[3])
{
  BGE_CORE_ASSERT(s_colliders.boxes.count < s_maxBoxCount,
                  "Max count colliders reached");

  uint32 collider = s_colliders.boxes.count++;

  memcpy(s_colliders.boxes.transforms[collider].position, position,
         sizeof(position[0]) * 3);

  memcpy(s_colliders.boxes.transforms[collider].rotation, rotation,
         sizeof(rotation[0]) * 4);

  memcpy(s_colliders.boxes.data[collider].size, size, sizeof(size[0]) * 3);

  s_colliders.boxes.tags[collider] = collider;

  return collider;
}

uint32 MakeSphereCollider(float position[3], float radius)
{
  BGE_CORE_ASSERT(s_colliders.spheres.count <= s_maxSphereCount,
                  "Max count colliders reached");

  uint32 collider = s_colliders.spheres.count++;

  s_colliders.spheres.transforms[collider] = s_identityTransform;

  memcpy(s_colliders.spheres.transforms[collider].position, position,
         sizeof(position[0]) * 3);

  s_colliders.spheres.data[collider].radius = radius;

  s_colliders.spheres.tags[collider] = collider + s_maxBoxCount;

  return collider;
}

uint32 MakeBoxBody(float mass, float cx, float cy, float cz)
{
  BGE_CORE_ASSERT(s_bodies.count < s_maxBodyCount, "Max count bodies reached");
  uint32 body = s_bodies.count++;

  float k = mass * (1.0f / 3.0f);

  float kcx2 = k * cx * cx;
  float kcy2 = k * cy * cy;
  float kcz2 = k * cz * cz;

  nudge::BodyProperties properties = {};
  properties.mass_inverse = 1.0f / mass;
  properties.inertia_inverse[0] = 1.0f / (kcy2 + kcz2);
  properties.inertia_inverse[1] = 1.0f / (kcx2 + kcz2);
  properties.inertia_inverse[2] = 1.0f / (kcx2 + kcy2);

  memset(&s_bodies.momentum[body], 0, sizeof(s_bodies.momentum[body]));
  s_bodies.idle_counters[body] = 0;
  s_bodies.properties[body] = properties;
  s_bodies.transforms[body] = s_identityTransform;

  return body;
}

uint32 MakeSphereBody(float mass, float radius)
{
  BGE_CORE_ASSERT(s_bodies.count < s_maxBodyCount, "Max count bodies reached");

  uint32 body = s_bodies.count++;

  float k = 2.5f / (mass * radius * radius);

  nudge::BodyProperties properties = {};
  properties.mass_inverse = 1.0f / mass;
  properties.inertia_inverse[0] = k;
  properties.inertia_inverse[1] = k;
  properties.inertia_inverse[2] = k;

  memset(&s_bodies.momentum[body], 0, sizeof(s_bodies.momentum[body]));
  s_bodies.idle_counters[body] = 0;
  s_bodies.properties[body] = properties;
  s_bodies.transforms[body] = s_identityTransform;

  return body;
}

Box CreateBox(float mass, float cx, float cy, float cz)
{
  BGE_CORE_ASSERT(s_bodies.count < s_maxBodyCount, "Max count bodies reached");
  BGE_CORE_ASSERT(s_colliders.boxes.count < s_maxBoxCount,
                  "Max count colliders reached");

  uint32 body = s_bodies.count++;
  uint32 collider = s_colliders.boxes.count++;

  float k = mass * (1.0f / 3.0f);

  float kcx2 = k * cx * cx;
  float kcy2 = k * cy * cy;
  float kcz2 = k * cz * cz;

  nudge::BodyProperties properties = {};
  properties.mass_inverse = 1.0f / mass;
  properties.inertia_inverse[0] = 1.0f / (kcy2 + kcz2);
  properties.inertia_inverse[1] = 1.0f / (kcx2 + kcz2);
  properties.inertia_inverse[2] = 1.0f / (kcx2 + kcy2);

  memset(&s_bodies.momentum[body], 0, sizeof(s_bodies.momentum[body]));
  s_bodies.idle_counters[body] = 0;
  s_bodies.properties[body] = properties;
  s_bodies.transforms[body] = s_identityTransform;

  s_colliders.boxes.transforms[collider] = s_identityTransform;
  s_colliders.boxes.transforms[collider].body = body;

  s_colliders.boxes.data[collider].size[0] = cx;
  s_colliders.boxes.data[collider].size[1] = cy;
  s_colliders.boxes.data[collider].size[2] = cz;
  s_colliders.boxes.tags[collider] = collider;

  return Box{collider, body};
}

Sphere CreateSphere(float mass, float radius)
{
  BGE_CORE_ASSERT(s_bodies.count < s_maxBodyCount, "Max count bodies reached");
  BGE_CORE_ASSERT(s_colliders.spheres.count < s_maxSphereCount,
                  "Max count colliders reached");

  uint32 body = s_bodies.count++;
  uint32 collider = s_colliders.spheres.count++;

  float k = 2.5f / (mass * radius * radius);

  nudge::BodyProperties properties = {};
  properties.mass_inverse = 1.0f / mass;
  properties.inertia_inverse[0] = k;
  properties.inertia_inverse[1] = k;
  properties.inertia_inverse[2] = k;

  memset(&s_bodies.momentum[body], 0, sizeof(s_bodies.momentum[body]));
  s_bodies.idle_counters[body] = 0;
  s_bodies.properties[body] = properties;
  s_bodies.transforms[body] = s_identityTransform;

  s_colliders.spheres.transforms[collider] = s_identityTransform;
  s_colliders.spheres.transforms[collider].body = body;

  s_colliders.spheres.data[collider].radius = radius;
  s_colliders.spheres.tags[collider] = collider + s_maxBoxCount;

  return Sphere{collider, body};
}

void SetBodyPosition(uint32 bodyId, float position[3])
{
  BGE_CORE_ASSERT(bodyId < s_bodies.count, "Invalid body Id");

  memcpy(s_bodies.transforms[bodyId].position, position,
         sizeof(position[0]) * 3);
}

void SetBoxColliderPosition(uint32 colliderId, float position[3])
{
  BGE_CORE_ASSERT(colliderId < s_colliders.boxes.count,
                  "Invalid box collider Id");

  memcpy(s_colliders.boxes.transforms[colliderId].position, position,
         sizeof(position[0]) * 3);
}

void SetBoxColliderSize(uint32 colliderId, float size[3])
{
  BGE_CORE_ASSERT(colliderId < s_colliders.boxes.count,
                  "Invalid box collider Id");

  memcpy(s_colliders.boxes.data[colliderId].size, size, sizeof(size[0]) * 3);
}

void SetBoxColliderBody(uint32 colliderId, uint32 bodyId)
{
  BGE_CORE_ASSERT(colliderId < s_colliders.boxes.count,
                  "Invalid box collider Id");

  s_colliders.boxes.transforms[colliderId].body = bodyId;
}

void SetSphereColliderPosition(uint32 colliderId, float position[3])
{
  BGE_CORE_ASSERT(colliderId < s_colliders.spheres.count,
                  "Invalid box collider Id");

  memcpy(s_colliders.spheres.transforms[colliderId].position, position,
         sizeof(position[0]) * 3);
}

void SetSphereColliderRadius(uint32 colliderId, float radius)
{
  BGE_CORE_ASSERT(colliderId < s_colliders.spheres.count,
                  "Invalid box collider Id");

  s_colliders.spheres.data[colliderId].radius = radius;
}

void SetSphereColliderBody(uint32 colliderId, uint32 bodyId)
{
  BGE_CORE_ASSERT(colliderId < s_colliders.spheres.count,
                  "Invalid box collider Id");

  s_colliders.spheres.transforms[colliderId].body = bodyId;
}

TransformsContainer GetAllBodiesTransforms()
{
  TransformsContainer result;

  memcpy(result.m_Transforms, s_bodies.transforms,
         sizeof(nudge::Transform) * s_bodies.count);

  result.m_Count = s_bodies.count;

  return result;
}

} // namespace PhysicsDevice
} // namespace bge
