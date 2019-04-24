#include "ecs/World.h"

#include "events/ECSEvents.h"

namespace bge
{

World::World()
    : m_EntityManager()
    , m_RenderWorld()
{
}

World::~World() {}

void World::SetEventCallback(const std::function<void(Event&)>& callback)
{
  m_EventCallback = callback;

  m_RenderWorld.SetEventCallback(callback);
  m_GameWorld.SetEventCallback(callback);
}

Entity World::CreateEntity() { return m_EntityManager.CreateEntity(); }

void World::DestroyEntity(Entity entity)
{
  m_EntityManager.DestroyEntity(entity);
  m_DestroyedEntities.push_back(entity);
}

void World::Update(float deltaTime)
{
  m_GameWorld.Tick(deltaTime);
  // m_audioWorld.Update();
  // m_physicsWorld.Update();
  EntitiesDestroyedEvent event(m_DestroyedEntities);

  // No need to use the event callback which sends the event to the "app layer"
  // For now, entity deletion only matters for the sub-worlds
  OnEvent(event);
}

void World::Render(float interpolation) { m_RenderWorld.Render(interpolation); }

void World::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  m_RenderWorld.OnEvent(event);
  m_GameWorld.OnEvent(event);
}

} // namespace bge