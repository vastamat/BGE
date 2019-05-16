#include "ecs/EntityManager.h"

#include "logging/Log.h"

namespace bge
{

static constexpr uint32 c_MinFreeIndices = 1024;

EntityManager::EntityManager()
{
  // Emplace the first entity which will act as the null (index of 0)
  m_EntityVersion.push_back(0u);
}

Entity EntityManager::CreateEntity()
{
  uint32 index = 0;

  if (m_FreeList.size() > c_MinFreeIndices)
  {
    index = m_FreeList.front();
    m_FreeList.pop_front();
  }
  else
  {
    m_EntityVersion.push_back(0u);
    index = m_EntityVersion.size() - 1;
    BGE_CORE_ASSERT(index < (1 << c_EntityIndexBits), "Entity Index Overflow");
  }

  return Entity(index, m_EntityVersion[index]);
}

void EntityManager::DestroyEntity(Entity id)
{
  BGE_CORE_ASSERT(IsAlive(id), "Entity is already destroyed");

  uint32 index = id.GetId();
  ++m_EntityVersion[index];
  m_FreeList.push_back(index);
}

} // namespace bge