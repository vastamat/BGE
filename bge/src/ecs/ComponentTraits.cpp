#include "ecs/ComponentTraits.h"

#include "logging/Log.h"

namespace bge
{

uint32 internal::GenerateUniqueTypeId()
{
  static uint32 s_uniqueComponentIdCounter = 0;
  return s_uniqueComponentIdCounter++;
}

} // namespace bge
