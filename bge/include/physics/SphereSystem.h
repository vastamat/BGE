#pragma once

#include "PhysicsDevice.h"

#include "ecs/ComponentTraits.h"
#include "logging/Log.h"

namespace bge
{

struct SphereData
{
  Sphere m_Sphere;
};

class SphereSystem
{
public:
  // Default template function to destroy just asserts as it should never be
  // called, only the explicit specializations in .cpp must be used
  template <typename T> void DestroyComponent(ComponentHandle handle)
  {
    BGE_CORE_ASSERT(false, "Trying to destroy an unknown component type");
  }

  // Same as above
  template <typename T> ComponentHandle AddComponent(const T& data)
  {
    BGE_CORE_ASSERT(false, "Trying to destroy an unknown component type");
    return ComponentHandle();
  }

  // Same
  template <typename T> T LookUpComponent(ComponentHandle handle)
  {
    BGE_CORE_ASSERT(false, "Trying to find an unknown component type");
    return nullptr;
  }

private:
  /// Component data array.
  std::vector<SphereData> m_Spheres;
  /// Component Id to Index mapping.
  std::vector<uint32> m_ComponentIdToIndex;
  /// Vector of component version numbers. Incremented each time an entity is
  /// destroyed
  std::vector<uint8> m_ComponentVersion;
  /// List of available entity slots.
  std::vector<uint32> m_FreeList;

  uint32 m_IndexCounter = 0;
};

} // namespace bge
