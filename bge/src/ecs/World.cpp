#include "ecs/World.h"

namespace bge
{

World::World()
    : m_EntityManager()
    , m_RenderWorld()
    , m_PhysicsWorld()
{
}

World::~World() {}

EntityId World::CreateEntity() { return m_EntityManager.CreateEntity(); }

void World::DestroyEntity(EntityId id) { m_EntityManager.DestroyEntity(id); }

Entity* World::LookUpEntity(EntityId id)
{
  return m_EntityManager.LookUpEntity(id);
}

void World::Update(float deltaTime)
{
  // m_gameWorld.Update();
  // m_audioWorld.Update();
  m_PhysicsWorld.Simulate();

  m_RenderWorld.UpdateTransforms(PhysicsDevice::GetAllBodiesTransforms());
}

void World::Render(float interpolation) { m_RenderWorld.Render(interpolation); }

void World::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<WindowCloseEvent>(
      BGE_BIND_EVENT_FN(World::OnWindowClose));
}

bool World::OnWindowClose(WindowCloseEvent& event)
{
  m_RenderWorld.OnExit();
  return true;
}

template <> RenderWorld* World::GetComponentWorld<RenderWorld>()
{
  return &m_RenderWorld;
}

template <> PhysicsWorld* World::GetComponentWorld<PhysicsWorld>()
{
  return &m_PhysicsWorld;
}

} // namespace bge