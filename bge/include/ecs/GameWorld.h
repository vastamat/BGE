#pragma once

#include "ecs/ComponentTraits.h"
#include "events/ECSEvents.h"
#include "logging/Log.h"

#include <map>
#include <memory>
#include <vector>

namespace bge
{

/**
 * Base game system which is to be inherited when creating custom
 * game systems for the application
 */
class GameSystem
{
public:
  GameSystem() = default;
  virtual ~GameSystem() = default;
  GameSystem(const GameSystem&) = default;
  GameSystem& operator=(const GameSystem&) = default;
  GameSystem(GameSystem&&) = default;
  GameSystem& operator=(GameSystem&&) = default;

  /**
   * virtual function called once every game update (every 40ms)
   */
  virtual void Tick(float deltaSeconds) {}

  /**
   * virtual function called when an event is broadcast
   */
  virtual void OnEvent(Event& event) {}
};

/**
 * The game sub-world which holds the custom game systems and calls them
 * during the game loop
 */
class GameWorld
{
public:
  /**
   * Sets the internal event callback variable
   * It's used to broadcast events to the application layer
   * @param callback the function pointer to use to broadcast events
   */
  void SetEventCallback(const std::function<void(Event&)>& callback);

  /**
   * Adds a unique custom game system to the list of systems
   * Does not support the adding of the same type more than once
   * @param system The derived game system
   * @return Pointer to the newly added system
   */
  template <typename T> T* AddGameSystem(std::unique_ptr<T> system)
  {
    static_assert(std::is_base_of<GameSystem, T>::value,
                  "Custom game systems must inherit from GameSystem");
    m_GameSystems.emplace_back(std::move(system));

    uint32 typeId = GetUniqueTypeId<T>();

    if (typeId >= m_SystemIdToArrayIndex.size())
    {
      // resize the index mapper to fit in the new system type
      m_SystemIdToArrayIndex.resize(typeId + 1, -1);
    }

    // If the index is not the default -1, then it's already been added
    BGE_CORE_ASSERT(m_SystemIdToArrayIndex[typeId] == -1,
                    "System of this type has already been added.");

    m_SystemIdToArrayIndex[typeId] = m_GameSystems.size() - 1;

    return static_cast<T*>(m_GameSystems.back().get());
  }

  /**
   * Returns a game system of desired type
   * @return Pointer to the desierd system
   */
  template <typename T> T* GetGameSystem()
  {
    uint32 typeId = GetUniqueTypeId<T>();
    BGE_CORE_ASSERT(typeId < m_SystemIdToArrayIndex.size() &&
                        m_SystemIdToArrayIndex[typeId] != -1,
                    "System type not added to the game world.");

    int32 arrayIndex = m_SystemIdToArrayIndex[typeId];

    return static_cast<T*>(m_GameSystems[arrayIndex].get());
  }

  /**
   * Calls the tick function of all added game systems
   * @param deltaSeconds the time passed since last update.
   * Always a fixed 0.040 seconds (25 FPS)
   */
  void Tick(float deltaSeconds);

  /**
   * Calls the OnEvent function of all added game systems
   * @param event the broadcast event
   */
  void OnEvent(Event& event);

private:
  /// array of custom game systems
  std::vector<std::unique_ptr<GameSystem>> m_GameSystems;
  /// Array which maps unique system id to array index
  std::vector<int32> m_SystemIdToArrayIndex;

  /// function pointer to broadcast events
  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge