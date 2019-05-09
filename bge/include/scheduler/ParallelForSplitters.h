#pragma once

#include "core/Common.h"

namespace bge
{

/////////////////////////////////////////////////
/// Splits the data based on the passed count.
/////////////////////////////////////////////////
class CountSplitter
{
public:
  explicit CountSplitter(uint32 count)
      : m_Count(count)
  {
    BGE_CORE_ASSERT(m_Count > 1, "Count must be higher than 1");
  }

  template <typename T> FORCEINLINE bool Split(uint32 count) const
  {
    return (count > m_Count);
  }

private:
  uint32 m_Count;
};

/////////////////////////////////////////////////
/// Splits the data based on the data size in bytes (this is useful for
/// splitting arrays into sizes that fit cache lines or the whole L1 cache)
/////////////////////////////////////////////////
class DataSizeSplitter
{
public:
  explicit DataSizeSplitter(uint32 size)
      : m_Size(size)
  {
    BGE_CORE_ASSERT(m_Size > 1, "Size must be more than 1");
  }

  template <typename T> FORCEINLINE bool Split(uint32 count) const
  {
    return (count * sizeof(T) > m_Size);
  }

private:
  uint32 m_Size;
};

} // namespace bge