#pragma once

#include "core/Common.h"

namespace bge
{

// helper template that yields the corresponding component type to any ID type
// at compile-time via an inner typedef named Type. specialized in other modules
// (graphics, game, ...)
template <typename T> struct ComponentIdToComponent
{
};

// helper template that yields the corresponding component ID type to any
// component type at compile-time via an inner typedef named Type. specialized
// in other modules (graphics, game, ...)
template <typename T> struct ComponentToComponentId
{
};

// helper struct that defines the corresponding system for each component ID in
// an inner Type typedef
template <typename T> struct ComponentIdToSystem
{
};

// helper struct that defines the corresponding world type for each component ID
// in an inner Type typedef
template <typename T> struct ComponentIdToWorld
{
};

// one counter for all different TypeToId templates.
// it is crucial that the struct itself is not a template, so that all TypeToId
// instantiations share the same static counter.
struct ComponentTypeRegister
{
  template <typename T> static uint32 RegisterFunctions()
  {
    static uint32 s_Counter;

    const uint32 index = s_Counter;
    ++s_Counter;

    return index;
  }
};

// each ID generates its own TypeToId template, but all template instantiations
// use the same global counter. therefore, the TypeToId::ID is a monotonically
// increasing value which is unique for each T. the values themselves don't
// matter, only that they are distinct for each type.
template <typename T> struct ComponentTypeToId
{
  static uint32 ID;
};

template <typename T>
uint32 ComponentTypeToId<T>::ID = ComponentTypeRegister::RegisterFunctions<T>();

struct ComponentHandle
{
  uint32 m_Index : 24;
  uint32 m_Generation : 8;
};

} // namespace bge