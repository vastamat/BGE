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

  Entity CreateEntity();
  void DestroyEntity(Entity id);

  void OnEvent(Event& event);
  bool OnWindowClose(WindowCloseEvent& event);

  FORCEINLINE RenderWorld& GetRenderWorld() { return m_RenderWorld; }

private:
  EntityManager m_EntityManager;

  RenderWorld m_RenderWorld;
};

} // namespace bge
