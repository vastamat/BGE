#pragma once

#include "Common.h"

#include "ecs/World.h"
#include "events/ApplicationEvents.h"
#include "video/Window.h"

#include <memory>

namespace bge
{

class Application
{
  /// Only 1 Engine Application can be instantiated
  static Application* s_Instance;

public:
  Application();
  virtual ~Application();

  DELETE_COPY_AND_ASSIGN(Application)

  void Run();

  void OnEvent(Event& event);

  FORCEINLINE static Application& Get() { return *s_Instance; }

  FORCEINLINE Window& GetWindow() { return m_Window; }

  FORCEINLINE World& GetWorld() { return m_World; }

private:
  bool OnWindowClose(WindowCloseEvent& event);

  World m_World;
  Window m_Window;

  bool m_Running = true;
};

} // namespace bge