#pragma once

#include "core/Common.h"

namespace bge
{

constexpr uint32 c_EntityIndexBits = 22u;
constexpr uint32 c_EntityIndexMask = (1u << c_EntityIndexBits) - 1u;
constexpr uint32 c_EntityGenerationBits = 8u;
constexpr uint32 c_EntityGenerationMask = (1u << c_EntityGenerationBits) - 1u;

/**
 * The entity class which is just a unique id that is used to implicitly
 * represent an object in the game through data held in systems
 */
class Entity
{
public:
  Entity(uint32 id, uint8 generation)
      : m_ID(generation)
  {
    m_ID = (m_ID << c_EntityIndexBits) | id;
  }

  /**
   * @return The unique entity id
   */
  FORCEINLINE uint32 GetId() const { return m_ID & c_EntityIndexMask; }

  /**
   * @return The generation of the entity
   */
  FORCEINLINE uint32 GetGeneration() const
  {
    return (m_ID >> c_EntityIndexBits) & c_EntityGenerationMask;
  }

  /**
   * @return flag to see if the entity is null
   */
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
  uint32 m_ID = 0u; /**< the id containing index & generation . */
};

} // namespace bge