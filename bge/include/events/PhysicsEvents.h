#pragma once

#include "Event.h"

#include "physics/PhysicsDevice.h"

namespace bge
{

/**
 * Event which represents the collided bodies in a single game state update
 */
class EntitiesCollidedEvent : public Event
{
public:
  EntitiesCollidedEvent(CollidedBodies collidedBodies)
      : m_CollidedBodies(std::move(collidedBodies))
  {
  }

  FORCEINLINE CollidedBodies& GetCollidedBodies() { return m_CollidedBodies; }

  EVENT_CLASS_TYPE(CollidedBodies)

private:
  CollidedBodies m_CollidedBodies; /**< The collided bodies of the update */
};

} // namespace bge