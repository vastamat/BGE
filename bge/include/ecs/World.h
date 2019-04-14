#pragma once

#include "ComponentTraits.h"
#include "EntityManager.h"
#include "events/ApplicationEvents.h"
#include "rendering/RenderWorld.h"

namespace bge
{

class World
{
public:
  World();
  ~World();

  void Update(float deltaTime);
  void Render(float interpolation);

  EntityId CreateEntity();
  void DestroyEntity(EntityId id);
  Entity* LookUpEntity(EntityId id);

  void OnEvent(Event& event);
  bool OnWindowClose(WindowCloseEvent& event);

  template <typename T>
  void DestroyComponent(EntityId& entity, ComponentHandle handle)
  {
    using WorldType = typename ComponentIdToWorld<T>::Type;

    LookUpEntity(entity)->RemoveComponent<T>();
    GetComponentWorld<WorldType>()->DestroyComponent(handle);
  }

  template <typename T>
  ComponentHandle AddComponent(EntityId& entity, const T& data)
  {
    using WorldType = typename ComponentIdToWorld<T>::Type;

    ComponentHandle handle = GetComponentWorld<WorldType>()->AddComponent(data);
    LookUpEntity(entity)->AddComponent<T>(handle);
    return handle;
  }

  template <typename T> T* LookUpComponent(ComponentHandle handle)
  {
    using WorldType = typename ComponentIdToWorld<T>::Type;
    return GetComponentWorld<WorldType>()->LookUpComponent(handle);
  }

  template <typename T> T* GetComponentWorld() { return nullptr; }

private:
  EntityManager m_EntityManager;

  RenderWorld m_RenderWorld;
};

} // namespace bge
