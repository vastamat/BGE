#pragma once

#include "GameComponents.h"

#include "events/ECSEvents.h"

#include <map>
#include <memory>
#include <vector>

namespace bge
{

class GameSystem
{
public:
  virtual ~GameSystem();
  DELETE_COPY_AND_ASSIGN(GameSystem)

  void AddComponentType(uint32 typeID);

  virtual void Tick(float deltaSeconds) {}
  virtual void OnEvent(Event& event) {}

  FORCEINLINE std::vector<Entity>& GetRegisteredEntities()
  {
    return m_RegisteredEntities;
  }

  FORCEINLINE const std::vector<uint32>& GetComponentTypes() const
  {
    return m_ComponentTypes;
  }

private:
  std::vector<Entity> m_RegisteredEntities;
  std::vector<uint32> m_ComponentTypes;
};

class GameWorld
{
public:
  void SetEventCallback(const std::function<void(Event&)>& callback);

  void AddGameSystem(std::unique_ptr<GameSystem> system);

  void Tick(float deltaSeconds);

  void OnEvent(Event& event);

  FORCEINLINE GameComponents& GetGameComponentsContainer()
  {
    return m_GameComponents;
  }

private:
  bool OnAddComponent(ComponentAddedEvent& event);
  bool OnRemoveComponent(ComponentRemovedEvent& event);

  std::map<Entity, std::vector<uint32>> m_EntityMasks;
  std::vector<std::unique_ptr<GameSystem>> m_GameSystems;
  GameComponents m_GameComponents;

  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge