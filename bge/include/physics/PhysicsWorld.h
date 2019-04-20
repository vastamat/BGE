#pragma once

#include "BoxSystem.h"
#include "SphereSystem.h"

#include "ecs/ComponentTraits.h"

#include <functional>
#include <vector>

namespace bge
{

class PhysicsWorld
{
  using OnCollideCallbackFn = std::function<void(uint16, uint16)>;

public:
  void Simulate();

  Box CreateBox(float mass, float cx, float cy, float cz);
  Sphere CreateSphere(float mass, float radius);

  void AddOnCollideEventHandler(const OnCollideCallbackFn& handler);

  template <typename T> void DestroyComponent(ComponentHandle handle)
  {
    using SystemType = typename ComponentIdToSystem<T>::Type;
    return GetComponentSystem<SystemType>()->DestroyComponent(handle);
  }

  template <typename T> ComponentHandle AddComponent(const T& data)
  {
    using SystemType = typename ComponentIdToSystem<T>::Type;
    return GetComponentSystem<SystemType>()->AddComponent(data);
  }

  template <typename T> T LookUpComponent(ComponentHandle handle)
  {
    using SystemType = typename ComponentIdToSystem<T>::Type;
    return GetComponentSystem<SystemType>()->LookUpComponent(handle);
  }

  template <typename T> T* GetComponentSystem() { return nullptr; }

private:
  BoxSystem m_BoxSystem;
  SphereSystem m_SphereSystem;

  std::vector<OnCollideCallbackFn> m_OnCollisionEvents;
};

template <> struct ComponentIdToWorld<BoxData>
{
  using WorldType = PhysicsWorld;
  using Type = WorldType;
};

template <> struct ComponentIdToSystem<BoxData>
{
  using SystemType = BoxSystem;
  using Type = SystemType;
};

template <> struct ComponentIdToWorld<SphereData>
{
  using WorldType = PhysicsWorld;
  using Type = WorldType;
};

template <> struct ComponentIdToSystem<SphereData>
{
  using SystemType = SphereSystem;
  using Type = SystemType;
};

} // namespace bge
