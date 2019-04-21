#pragma once

#include "Entity.h"

#include <deque>
#include <vector>

namespace bge
{

class EntityManager
{
public:
  Entity CreateEntity();
  void DestroyEntity(Entity id);

  FORCEINLINE bool IsAlive(Entity entity) const
  {
    return m_EntityVersion[entity.GetId()] == entity.GetGeneration();
  }

private:
  std::vector<uint8> m_EntityVersion;
  /// List of available entity slots.
  std::deque<uint32> m_FreeList;
};

} // namespace bge
