#pragma once

#include "core/Common.h"

namespace bge
{

constexpr int c_IndexBits = 24;
constexpr int c_GenerationBits = 8;

struct BGE_API Entity
{
  uint32 m_Index : c_IndexBits;
  uint32 m_Generation : c_GenerationBits;

  FORCEINLINE uint32 GetId() const { return m_Index; }
  FORCEINLINE uint32 GetGeneration() const { return m_Generation; }

  FORCEINLINE bool IsNull() const { return m_Index == 0u; }

  FORCEINLINE bool operator==(const Entity& _Rhs) const
  {
    return m_Index == _Rhs.m_Index;
  }
  FORCEINLINE bool operator!=(const Entity& _Rhs) const
  {
    return m_Index != _Rhs.m_Index;
  }
};

} // namespace bge