#pragma once

#include "ComponentTraits.h"

#include "logging/Log.h"

#include <vector>

namespace bge
{

/* When spawning a component, each subworld simply returns the ID of that
 * component, which gets stored inside the entity.*/

/* The entity itself just stores two integer arrays: one containing the
 * component IDs, one containing the component types.*/
class Entity
{
public:
  template <typename T> void AddComponent(ComponentHandle handle)
  {
    m_ComponentIDs.push_back(handle);
    m_ComponentTypes.push_back(ComponentTypeToId<T>::ID);
  }

  template <typename T> void RemoveComponent()
  {
    for (size_t i = 0; i < m_ComponentTypes.size(); i++)
    {
      if (m_ComponentTypes[i] == ComponentTypeToId<T>::ID)
      {
        std::swap(m_ComponentIDs[i], m_ComponentIDs[m_ComponentIDs.size() - 1]);
        m_ComponentIDs.pop_back();

        std::swap(m_ComponentTypes[i],
                  m_ComponentTypes[m_ComponentTypes.size() - 1]);
        m_ComponentTypes.pop_back();
      }
    }
  }

  template <typename T> ComponentHandle GetComponentHandle()
  {
    for (size_t i = 0; i < m_ComponentTypes.size(); i++)
    {
      if (m_ComponentTypes[i] == ComponentTypeToId<T>::ID)
      {
        return m_ComponentIDs[i];
      }
    }

    BGE_CORE_ASSERT(false, "Unable to find component of ID: {0} in entity.",
                    ComponentTypeToId<T>::ID);

    return ComponentHandle();
  }

private:
  std::vector<ComponentHandle> m_ComponentIDs;
  std::vector<uint16> m_ComponentTypes;
};

// Struct to identify an entity in the world
struct EntityId
{
  uint32 m_Index : 24;
  uint32 m_Generation : 8;
};

} // namespace bge