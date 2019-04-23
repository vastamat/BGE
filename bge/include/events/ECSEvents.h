#pragma once

#include "Event.h"

namespace bge
{

class ECSEvent : public Event
{
public:
  ECSEvent(Entity entity, uint32 componentTypeId)
      : m_Entity(entity)
      , m_ComponentTypeId(componentTypeId)
  {
  }

  FORCEINLINE Entity GetEntity() const noexcept { return m_Entity; }
  FORCEINLINE uint32 GetComponentID() const noexcept
  {
    return m_ComponentTypeId;
  }

private:
  Entity m_Entity;
  uint32 m_ComponentTypeId;
};

class ComponentAddedEvent : public ECSEvent
{
public:
  ComponentAddedEvent(Entity entity, uint32 componentTypeId)
      : ECSEvent(entity, componentTypeId)
  {
  }

  EVENT_CLASS_TYPE(ComponentAdded)
};

class ComponentRemovedEvent : public ECSEvent
{
public:
  ComponentRemovedEvent(Entity entity, uint32 componentTypeId)
      : ECSEvent(entity, componentTypeId)
  {
  }

  EVENT_CLASS_TYPE(ComponentRemoved)
};

} // namespace bge