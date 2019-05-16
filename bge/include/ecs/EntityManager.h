#pragma once

#include "Entity.h"

#include <deque>
#include <vector>

namespace bge
{

/**
 * The class which handles creation and destruction of entities in the engine
 */
class EntityManager
{
public:
  EntityManager();

  /**
   * Create a new entity
   * @return the entity
   */
  Entity CreateEntity();

  /**
   * Destroy an existing entity
   * @param id the entity to destroy
   */
  void DestroyEntity(Entity id);

  /**
   * Check if an entity is alive
   * @param id the entity to check
   * @return flag whether an entity is alive
   */
  FORCEINLINE bool IsAlive(Entity entity) const
  {
    return m_EntityVersion[entity.GetId()] == entity.GetGeneration();
  }

private:
  std::vector<uint8> m_EntityVersion; /**< vector of entity generations */
  std::deque<uint32> m_FreeList;      /**< List of available entity slots. */
};

} // namespace bge
