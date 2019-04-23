#pragma once

#include "ComponentTraits.h"
#include "Entity.h"

#include "events/ECSEvents.h"
#include "logging/Log.h"

#include <map>
#include <memory>
#include <vector>

namespace bge
{

class BaseComponentManager
{
public:
  BaseComponentManager() = default;
  virtual ~BaseComponentManager() = default;
  BaseComponentManager(const BaseComponentManager&) = default;
  BaseComponentManager& operator=(const BaseComponentManager&) = default;
  BaseComponentManager(BaseComponentManager&&) = default;
  BaseComponentManager& operator=(BaseComponentManager&&) = default;

  virtual void RemoveComponent(Entity entity) = 0;
};

template <typename ComponentType>
class ComponentManager : public BaseComponentManager
{
public:
  ComponentManager()
      : m_EntityIdToComponentIndex()
      , m_Components()
  {
    m_Components.reserve(64);
  }

  ComponentType& AddComponent(Entity entity, ComponentType&& component)
  {
    BGE_CORE_ASSERT(m_EntityIdToComponentIndex.count(entity.GetId()) == 0,
                    "Component already exists for this entity");

    m_Components.emplace_back(std::move(component));
    m_EntityIdToComponentIndex[entity.GetId()] = m_Components.size() - 1;
    return m_Components.back();
  }

  void RemoveComponent(Entity entity) override
  {
    auto componentIndexToRemove = m_EntityIdToComponentIndex[entity.GetId()];
    auto lastComponentIndex = m_Components.size() - 1;

    m_Components[componentIndexToRemove] = m_Components[lastComponentIndex];

    m_Components.pop_back();

    for (auto&& entry : m_EntityIdToComponentIndex)
    {
      if (entry.second == lastComponentIndex)
      {
        entry.second = componentIndexToRemove;
        break;
      }
    }

    m_EntityIdToComponentIndex.erase(entity.GetId());
  }

  FORCEINLINE ComponentType* GetComponent(Entity entity)
  {
    BGE_CORE_ASSERT(m_EntityIdToComponentIndex.count(entity.GetId()) == 1,
                    "Component does not exist for this entity")

    return &m_Components[m_EntityIdToComponentIndex[entity.GetId()]];
  }

  FORCEINLINE std::vector<ComponentType>& GetAllComponents()
  {
    return m_Components;
  }

private:
  std::unordered_map<uint32, uint32> m_EntityIdToComponentIndex;
  std::vector<ComponentType> m_Components;
};

class GameComponents
{
public:
  void SetEventCallback(const std::function<void(Event&)>& callback)
  {
    m_EventCallback = callback;
  }

  template <typename ComponentType>
  void AddCustomComponentManager(
      std::unique_ptr<ComponentManager<ComponentType>> manager)
  {
    uint32 typeId = GetUniqueTypeId<ComponentType>();

    BGE_CORE_ASSERT(m_ComponentManagers.count(typeId) == 0,
                    "Component Manager already added.");

    m_ComponentManagers[typeId] = std::move(manager);
  }

  template <typename ComponentType>
  FORCEINLINE void AddComponent(Entity entity, ComponentType&& component)
  {
    ComponentManager<ComponentType>* manager =
        GetComponentManager<ComponentType>();
    manager->AddComponent(entity, component);

    ComponentAddedEvent event(entity, GetUniqueTypeId<ComponentType>());
    m_EventCallback(event);
  }

  template <typename ComponentType>
  FORCEINLINE void RemoveComponent(Entity entity)
  {
    ComponentManager<ComponentType>* manager =
        GetComponentManager<ComponentType>();
    manager->RemoveComponent(entity);

    ComponentRemovedEvent event(entity, GetUniqueTypeId<ComponentType>());
    m_EventCallback(event);
  }

  FORCEINLINE void RemoveComponentWithId(Entity entity, uint32 componentId)
  {
    BaseComponentManager* manager = GetComponentManagerWithId(componentId);
    manager->RemoveComponent(entity);

    ComponentRemovedEvent event(entity, componentId);
    m_EventCallback(event);
  }

  template <typename ComponentType>
  FORCEINLINE ComponentType* GetComponent(Entity entity)
  {
    ComponentManager<ComponentType>* manager =
        GetComponentManager<ComponentType>();

    return manager->GetComponent(entity);
  }

  template <typename ComponentType>
  FORCEINLINE std::vector<ComponentType>& GetAllComponents()
  {
    ComponentManager<ComponentType>* manager =
        GetComponentManager<ComponentType>();

    return manager->GetAllComponents();
  }

private:
  template <typename ComponentType>
  ComponentManager<ComponentType>* GetComponentManager()
  {
    uint32 typeId = GetUniqueTypeId<ComponentType>();

    BGE_CORE_ASSERT(m_ComponentManagers.count(typeId) == 1,
                    "Component Manager does not exist.");

    return static_cast<ComponentManager<ComponentType>*>(
        m_ComponentManagers[typeId].get());
  }

  BaseComponentManager* GetComponentManagerWithId(uint32 componentTypeId)
  {
    BGE_CORE_ASSERT(m_ComponentManagers.count(componentTypeId) == 1,
                    "Component Manager does not exist.");

    return m_ComponentManagers[componentTypeId].get();
  }

private:
  std::map<uint32, std::unique_ptr<BaseComponentManager>> m_ComponentManagers;
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge
