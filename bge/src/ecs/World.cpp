#include "ecs/World.h"

namespace bge
{

World::World()
    : m_EntityManager()
    , m_RenderWorld()
{
}

World::~World() {}

Entity World::CreateEntity() { return m_EntityManager.CreateEntity(); }

void World::DestroyEntity(Entity id) { m_EntityManager.DestroyEntity(id); }

void World::Update(float deltaTime)
{
  // m_gameWorld.Update();
  // m_audioWorld.Update();
  // m_physicsWorld.Update();
}

void World::Render(float interpolation) { m_RenderWorld.Render(interpolation); }

void World::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<WindowCloseEvent>(
      BGE_BIND_EVENT_FN(World::OnWindowClose));
}

bool World::OnWindowClose(WindowCloseEvent& event) { m_RenderWorld.OnExit(); }

} // namespace bge