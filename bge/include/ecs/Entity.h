#pragma once

#include "core/Common.h"

namespace bge
{

constexpr uint32 c_EntityIndexBits = 22u;
constexpr uint32 c_EntityIndexMask = (1u << c_EntityIndexBits) - 1u;
constexpr uint32 c_EntityGenerationBits = 8u;
constexpr uint32 c_EntityGenerationMask = (1u << c_EntityGenerationBits) - 1u;

class Entity
{
public:
  Entity(uint32 id, uint8 generation)
      : m_ID(generation)
  {
    m_ID = (m_ID << c_EntityIndexBits) | id;
  }

  FORCEINLINE uint32 GetId() const { return m_ID & c_EntityIndexMask; }
  FORCEINLINE uint32 GetGeneration() const
  {
    return (m_ID >> c_EntityIndexBits) & c_EntityGenerationMask;
  }

  FORCEINLINE bool IsNull() const { return m_ID == 0u; }

  FORCEINLINE bool operator==(const Entity& rhs) const
  {
    return m_ID == rhs.m_ID;
  }
  FORCEINLINE bool operator!=(const Entity& rhs) const
  {
    return m_ID != rhs.m_ID;
  }
  FORCEINLINE bool operator<(const Entity& rhs) const
  {
    return m_ID < rhs.m_ID;
  }

private:
  uint32 m_ID = 0u;
};

} // namespace bge