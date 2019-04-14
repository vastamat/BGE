#pragma once

#include "Entity.h"

namespace bge
{

class EntityManager
{
public:
  EntityId CreateEntity();
  void DestroyEntity(EntityId id);

  Entity* LookUpEntity(EntityId id);

private:
  /// Component data array.
  std::vector<Entity> m_Entities;
  /// Component Id to Index mapping.
  std::vector<uint32> m_EntityIdToIndex;
  /// Vector of component version numbers. Incremented each time an entity is
  /// destroyed
  std::vector<uint8> m_EntityVersion;
  /// List of available entity slots.
  std::vector<uint32> m_FreeList;

  uint32 m_IndexCounter = 0;
};

} // namespace bge
