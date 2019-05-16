#pragma once

#include "Common.h"

#include "ecs/World.h"
#include "events/ApplicationEvents.h"
#include "video/Window.h"

#include <memory>

namespace bge
{

/**
 * The application class which must be extended by the user to create their app.
 */
class Application
{
  /// Only 1 Engine Application can be instantiated
  static Application* s_Instance;

public:
  Application();
  virtual ~Application();

  DELETE_COPY_AND_ASSIGN(Application)

  /**
   * Runs the application
   */
  void Run();

  /**
   * Receiver of broadcast events
   * @param event the broadcast event
   */
  void OnEvent(Event& event);

  /**
   * @return The application instance
   */
  FORCEINLINE static Application& Get() { return *s_Instance; }

  /**
   * @return The application window
   */
  FORCEINLINE Window& GetWindow() { return m_Window; }

  /**
   * @return The main world
   */
  FORCEINLINE World& GetWorld() { return m_World; }

private:
  /**
   * Event handler for the window close event
   * @param event the broadcast event
   */
  bool OnWindowClose(WindowCloseEvent& event);

  World m_World;   /**< the main world . */
  Window m_Window; /**< the app window . */

  bool m_Running = true; /**< game loop flag . */
};

} // namespace bge