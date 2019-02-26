#include "ecs/EntityManager.h"

#include "logging/Log.h"

namespace bge
{

Entity EntityManager::CreateEntity()
{
  uint32 index = 0;

  if (m_FreeIndices.size() > c_MinimumFreeIndices)
  {
    index = m_FreeIndices.front();
    m_FreeIndices.pop_front();
  }
  else
  {
    m_Generations.push_back(0);
    index = m_Generations.size() - 1;
    BGE_CORE_ASSERT(index < (1 << c_IndexBits),
                    "Index is over the maximum allowed amount.");
  }
  return MakeEntity(index, m_Generations[index]);
}

} // namespace bge