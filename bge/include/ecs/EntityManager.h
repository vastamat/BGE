#pragma once

#include "Entity.h"

#include <deque>
#include <vector>

namespace bge
{

constexpr uint32 c_MinimumFreeIndices = 1024;

class BGE_API EntityManager
{
public:
  Entity CreateEntity();

  FORCEINLINE bool IsAlive(Entity entity)
  {
    return m_Generations[entity.m_Index] == entity.m_Generation;
  }

  FORCEINLINE void Destroy(Entity entity)
  {
    const uint32 index = entity.m_Index;
    ++m_Generations[index];
    m_FreeIndices.push_back(index);
  }

private:
  FORCEINLINE Entity MakeEntity(uint32 index, uint8 generation)
  {
    Entity newEntity;
    newEntity.m_Index = index;
    newEntity.m_Generation = generation;
    return newEntity;
  }

  std::vector<uint8> m_Generations;
  std::deque<uint32> m_FreeIndices;
};

} // namespace bge