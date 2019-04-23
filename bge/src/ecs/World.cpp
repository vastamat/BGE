#include "ecs/World.h"

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
}

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