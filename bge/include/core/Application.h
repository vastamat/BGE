#pragma once

#include "Common.h"
#include "events/ApplicationEvents.h"
#include "video/Window.h"

#include <memory>

namespace bge
{

class BGE_API Application
{
  /// Only 1 Engine Application can be instantiated
  static Application* s_Instance;

public:
  Application();
  virtual ~Application();

  void Run();

  void OnEvent(Event& event);

  FORCEINLINE static Application& Get() { return *s_Instance; }

private:
  bool OnWindowClose(WindowCloseEvent& event);

  Window m_Window;
  bool m_Running = true;
};

// To be defined in CLIENT
std::unique_ptr<Application> CreateApplication();

} // namespace bge