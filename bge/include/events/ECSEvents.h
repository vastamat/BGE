#pragma once

#include "Event.h"

#include "ecs/Entity.h"

#include <vector>

namespace bge
{

/**
 * Base event for when a component is changed in an entity
 */
class ComponentChangeEvent : public Event
{
public:
  ComponentChangeEvent(Entity entity, uint32 componentTypeId)
      : m_Entity(entity)
      , m_ComponentTypeId(componentTypeId)
  {
  }
  virtual ~ComponentChangeEvent() {}

  FORCEINLINE Entity GetEntity() const noexcept { return m_Entity; }
  FORCEINLINE uint32 GetComponentID() const noexcept
  {
    return m_ComponentTypeId;
  }

private:
  Entity m_Entity;          /**< The entity whose had a change */
  uint32 m_ComponentTypeId; /**< The component that was added or removed */
};

/**
 * Event for when a component is added
 */
class ComponentAddedEvent : public ComponentChangeEvent
{
public:
  ComponentAddedEvent(Entity entity, uint32 componentTypeId)
      : ComponentChangeEvent(entity, componentTypeId)
  {
  }

  EVENT_CLASS_TYPE(ComponentAdded)
};

/**
 * Event for when a component is removed
 */
class ComponentRemovedEvent : public ComponentChangeEvent
{
public:
  ComponentRemovedEvent(Entity entity, uint32 componentTypeId)
      : ComponentChangeEvent(entity, componentTypeId)
  {
  }

  EVENT_CLASS_TYPE(ComponentRemoved)
};

/**
 * Event which contains all entities which have been destroyed in a single game
 * state update
 */
class EntitiesDestroyedEvent : public Event
{
public:
  EntitiesDestroyedEvent(const std::vector<Entity>& entities)
      : m_Entities(entities)
  {
  }

  FORCEINLINE const std::vector<Entity>& GetEntities() const noexcept
  {
    return m_Entities;
  }

  EVENT_CLASS_TYPE(EntitiesDestroyed)

private:
  std::vector<Entity> m_Entities; /**< The list of entities which are dead */
};

} // namespace bge