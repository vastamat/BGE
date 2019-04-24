#pragma once

#include "ecs/ComponentTraits.h"
#include "events/ECSEvents.h"
#include "logging/Log.h"

#include <map>
#include <memory>
#include <vector>

namespace bge
{

class GameSystem
{
public:
  GameSystem() = default;
  virtual ~GameSystem() = default;
  GameSystem(const GameSystem&) = default;
  GameSystem& operator=(const GameSystem&) = default;
  GameSystem(GameSystem&&) = default;
  GameSystem& operator=(GameSystem&&) = default;

  virtual void Tick(float deltaSeconds) {}
  virtual void OnEvent(Event& event) {}
};

class GameWorld
{
public:
  void SetEventCallback(const std::function<void(Event&)>& callback);

  template <typename T> T* AddGameSystem(std::unique_ptr<T> system)
  {
    static_assert(std::is_base_of<GameSystem, T>::value,
                  "Custom game systems must inherit from GameSystem");
    m_GameSystems.emplace_back(std::move(system));

    uint32 typeId = GetUniqueTypeId<T>();

    BGE_CORE_ASSERT(typeId >= m_SystemIdToArrayIndex.size(),
                    "System of this type has already been added.");

    m_SystemIdToArrayIndex.resize(typeId + 1);
    m_SystemIdToArrayIndex[typeId] = m_GameSystems.size() - 1;

    return static_cast<T*>(m_GameSystems.back().get());
  }

  template <typename T> T* GetGameSystem()
  {
    uint32 typeId = GetUniqueTypeId<T>();
    BGE_CORE_ASSERT(typeId < m_SystemIdToArrayIndex.size(),
                    "System type not added to the game world.");

    uint32 arrayIndex = m_SystemIdToArrayIndex[typeId];

    return static_cast<T*>(m_GameSystems[arrayIndex].get());
  }

  void Tick(float deltaSeconds);

  void OnEvent(Event& event);

private:
  std::vector<std::unique_ptr<GameSystem>> m_GameSystems;
  std::vector<uint32> m_SystemIdToArrayIndex;

  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge