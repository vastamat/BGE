#pragma once

#include "core/Common.h"

#include <type_traits>

namespace bge
{

namespace internal
{

/**
 * Generates a unique conponent Id by incrementing each call
 */
uint32 GenerateUniqueTypeId();

} // namespace internal

/**
 * @return unique id for the templated type
 */
template <typename T> uint32 GetUniqueTypeId()
{
  static uint32 s_ComponentTypeId = internal::GenerateUniqueTypeId();
  return s_ComponentTypeId;
}

} // namespace bge