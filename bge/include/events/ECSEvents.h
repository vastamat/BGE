#pragma once

#include "Event.h"

#include <vector>

namespace bge
{

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
  Entity m_Entity;
  uint32 m_ComponentTypeId;
};

class ComponentAddedEvent : public ComponentChangeEvent
{
public:
  ComponentAddedEvent(Entity entity, uint32 componentTypeId)
      : ComponentChangeEvent(entity, componentTypeId)
  {
  }

  EVENT_CLASS_TYPE(ComponentAdded)
};

class ComponentRemovedEvent : public ComponentChangeEvent
{
public:
  ComponentRemovedEvent(Entity entity, uint32 componentTypeId)
      : ComponentChangeEvent(entity, componentTypeId)
  {
  }

  EVENT_CLASS_TYPE(ComponentRemoved)
};

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
  std::vector<Entity> m_Entities;
};

} // namespace bge