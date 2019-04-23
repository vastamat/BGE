#pragma once

#include "ComponentTraits.h"
#include "EntityManager.h"
#include "GameWorld.h"

#include "events/Event.h"
#include "rendering/RenderWorld.h"

namespace bge
{

class World
{
public:
  World();
  ~World();

  void SetEventCallback(const std::function<void(Event&)>& callback);

  void Update(float deltaTime);
  void Render(float interpolation);

  Entity CreateEntity();
  void DestroyEntity(Entity entity);

  void OnEvent(Event& event);

  FORCEINLINE RenderWorld& GetRenderWorld() { return m_RenderWorld; }
  FORCEINLINE GameWorld& GetGameWorld() { return m_GameWorld; }

private:
  EntityManager m_EntityManager;

  RenderWorld m_RenderWorld;
  GameWorld m_GameWorld;

  std::vector<Entity> m_DestroyedEntities;

  std::function<void(Event&)> m_EventCallback;
};

} // namespace bge
