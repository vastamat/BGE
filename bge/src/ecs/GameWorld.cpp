#include "ecs/GameWorld.h"

#include <functional>

namespace bge
{

template <typename T>
bool VectorContains(T element, const std::vector<T> container)
{
  for (size_t i = 0; i < container.size(); i++)
  {
    if (element == container[i])
    {
      return true;
    }
  }

  return false;
}

void GameSystem::AddComponentType(uint32 typeID)
{
  m_ComponentTypes.push_back(typeID);
}

void GameWorld::SetEventCallback(const std::function<void(Event&)>& callback)
{
  m_EventCallback = callback;

  m_GameComponents.SetEventCallback(callback);
}

void GameWorld::AddGameSystem(std::unique_ptr<GameSystem> system)
{
  m_GameSystems.emplace_back(std::move(system));
}

void GameWorld::Tick(float deltaSeconds)
{
  for (auto&& system : m_GameSystems)
  {
    system->Tick(deltaSeconds);
  }
}

void GameWorld::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<ComponentAddedEvent>(
      BGE_BIND_EVENT_FN(GameWorld::OnAddComponent));
  dispatcher.Dispatch<ComponentRemovedEvent>(
      BGE_BIND_EVENT_FN(GameWorld::OnRemoveComponent));
  dispatcher.Dispatch<EntitiesDestroyedEvent>(
      BGE_BIND_EVENT_FN(GameWorld::OnEntitiesDestroyed));

  for (auto&& system : m_GameSystems)
  {
    system->OnEvent(event);
    if (event.m_Handled)
    {
      break;
    }
  }
}

bool GameWorld::OnAddComponent(ComponentAddedEvent& event)
{
  Entity entity = event.GetEntity();
  uint32 componentId = event.GetComponentID();

  m_EntityMasks[entity].push_back(componentId);

  for (auto& system : m_GameSystems)
  {
    const auto& systemComponentTypes = system->GetComponentTypes();
    auto& registeredEntities = system->GetRegisteredEntities();

    // if the entity is not registered in this system
    if (!VectorContains(entity, registeredEntities))
    {
      // and if the system's required component type matches with the new mask
      bool matches = true;
      for (size_t i = 0; i < systemComponentTypes.size(); i++)
      {
        if (!VectorContains(systemComponentTypes[i], m_EntityMasks[entity]))
        {
          matches = false;
          break;
        }
      }

      if (matches)
      {
        registeredEntities.push_back(entity);
      }
    }
  }

  return false;
}

bool GameWorld::OnRemoveComponent(ComponentRemovedEvent& event)
{
  Entity entity = event.GetEntity();
  uint32 componentId = event.GetComponentID();

  auto& entityMask = m_EntityMasks[entity];
  for (size_t i = 0; i < entityMask.size(); i++)
  {
    if (entityMask[i] == componentId)
    {
      entityMask[i] = entityMask.back();
      entityMask.pop_back();
      break;
    }
  }

  for (auto& system : m_GameSystems)
  {
    const auto& systemComponentTypes = system->GetComponentTypes();
    auto& registeredEntities = system->GetRegisteredEntities();

    // if the entity is not registered in this system
    if (VectorContains(entity, registeredEntities))
    {
      // and if the system's required component type matches with the new mask
      bool noLongerMatches = false;
      for (size_t i = 0; i < systemComponentTypes.size(); i++)
      {
        if (!VectorContains(systemComponentTypes[i], m_EntityMasks[entity]))
        {
          noLongerMatches = true;
          break;
        }
      }

      if (noLongerMatches)
      {
        for (size_t i = 0; i < registeredEntities.size(); i++)
        {
          if (registeredEntities[i] == entity)
          {
            registeredEntities[i] = registeredEntities.back();
            registeredEntities.pop_back();
            break;
          }
        }
      }
    }
  }

  return false;
}

bool GameWorld::OnEntitiesDestroyed(EntitiesDestroyedEvent& event)
{
  for (auto&& entity : event.GetEntities())
  {
    const auto& entityMask = m_EntityMasks[entity];

    // Go Through all the components the entity has and remove them
    for (auto&& componentID : entityMask)
    {
      // This will break when engine components are in the entity, currently
      m_GameComponents.RemoveComponentWithId(entity, componentID);
    }

    // remove the entity from the systems that track it
    for (auto&& system : m_GameSystems)
    {
      auto& registeredEntities = system->GetRegisteredEntities();

      for (size_t i = 0; i < registeredEntities.size(); ++i)
      {
        if (registeredEntities[i] == entity)
        {
          registeredEntities[i] = registeredEntities.back();
          registeredEntities.pop_back();
          break;
        }
      }
    }

    // for (auto&& existingEntity : m_Entities)
    // {
    //   if (entity == existingEntity)
    //   {
    //     DestroyComponent(entity);
    //   }
    // }
  }

  return false;
}

} // namespace bge
