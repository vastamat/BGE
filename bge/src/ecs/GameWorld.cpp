#include "ecs/GameWorld.h"

#include <functional>

namespace bge
{

void GameWorld::SetEventCallback(const std::function<void(Event&)>& callback)
{
  m_EventCallback = callback;
}

void GameWorld::Tick(float deltaSeconds)
{
  for (auto&& system : m_GameSystems)
  {
    system->Tick(deltaSeconds);
  }
}

void GameWorld::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  for (auto&& system : m_GameSystems)
  {
    system->OnEvent(event);
    if (event.m_Handled)
    {
      break;
    }
  }
}

} // namespace bge
