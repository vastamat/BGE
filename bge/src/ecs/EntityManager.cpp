#include "ecs/EntityManager.h"

namespace bge
{

EntityId EntityManager::CreateEntity()
{
  EntityId handle;
  if (m_FreeList.empty())
  {
    m_Entities.emplace_back();
    m_EntityIdToIndex.emplace_back(m_IndexCounter);
    m_EntityVersion.emplace_back(0u);
    handle.m_Index = m_IndexCounter++;
    handle.m_Generation = 0u;
  }
  else
  {
    uint32 freeSlot = m_FreeList.back();
    m_FreeList.pop_back();
    m_Entities.emplace_back();
    m_EntityIdToIndex[freeSlot] = m_Entities.size() - 1;
    handle.m_Index = freeSlot;
    handle.m_Generation = m_EntityVersion[freeSlot];
  }
  return handle;
}

void EntityManager::DestroyEntity(EntityId id)
{
  // assert this operation is valid
  BGE_CORE_ASSERT(m_EntityVersion[id.m_Index] == id.m_Generation,
                  "Trying to destroy dead entity.");

  // Get the index to the component in the internal array
  uint32 compIndex = m_EntityIdToIndex[id.m_Index];
  uint32 lastIndex = m_Entities.size() - 1;
  // swap the component to be removed with the last added component and pop
  std::swap(m_Entities[compIndex], m_Entities[lastIndex]);
  m_Entities.pop_back();

  // Update the mapping so the id which pointed to the last component now points
  // to the initial index where the component was swapped to
  for (size_t i = 0; i < m_EntityIdToIndex.size(); i++)
  {
    // Find which mapping instance "points" to the index of the last component
    // in the array
    if (m_EntityIdToIndex[i] == lastIndex)
    {
      // set that instance to point to the removed component index where the
      // data was swapped to
      m_EntityIdToIndex[i] = compIndex;
      break;
    }
  }

  // Set the removed component mapping to max uint so that it doesn't point to
  // any valid data instances which could affect the loop above.
  m_EntityIdToIndex[id.m_Index] = UINT_MAX;
  // Add the mapping which has been removed to the free list
  m_FreeList.push_back(id.m_Index);

  // Update the generation to invalidate the mapping to the removed component
  ++m_EntityVersion[id.m_Index];
}

Entity* EntityManager::LookUpEntity(EntityId id)
{
  assert(m_EntityVersion[id.m_Index] == id.m_Generation);
  return &m_Entities[m_EntityIdToIndex[id.m_Index]];
}

} // namespace bge