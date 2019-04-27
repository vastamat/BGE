#pragma once

#include "ComponentTraits.h"
#include "EntityManager.h"
#include "GameWorld.h"

#include "events/Event.h"
#include "physics/PhysicsWorld.h"
#include "rendering/RenderWorld.h"

namespace bge
{

class World
{
public:
  World();

  void Init();

  void SetEventCallback(const std::function<void(Event&)>& callback);

  void Update(float deltaTime);
  void Render(float interpolation);

  Entity CreateEntity();
  void DestroyEntity(Entity entity);

  void OnEvent(Event& event);

  FORCEINLINE RenderWorld& GetRenderWorld() { return m_RenderWorld; }
  FORCEINLINE GameWorld& GetGameWorld() { return m_GameWorld; }
  FORCEINLINE PhysicsWorld& GetPhysicsWorld() { return m_PhysicsWorld; }

private:
  EntityManager m_EntityManager;

  RenderWorld m_RenderWorld;
  PhysicsWorld m_PhysicsWorld;
  GameWorld m_GameWorld;

  std::vector<Entity> m_DestroyedEntities;

  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge
