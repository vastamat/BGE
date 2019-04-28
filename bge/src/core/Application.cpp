#include "core/Application.h"

#include "logging/Log.h"
#include "physics/PhysicsDevice.h"
#include "rendering/RenderDevice.h"
#include "scheduler/Scheduler.h"
#include "util/Timer.h"

#include <functional>
#include <thread>

namespace bge
{

// Fixed update frame limiter
constexpr int32 c_DesiredUpdateFPS = 25;
constexpr int32 c_MillisPerSecond = 1000;
constexpr int32 c_DesiredUpdateFrameMS = c_MillisPerSecond / c_DesiredUpdateFPS;
constexpr int32 c_MaxUpdatesPerFrame = 5;
constexpr float c_FixedUpdateDeltaSeconds = c_DesiredUpdateFrameMS / 1000.0f;

// Variable Render frame limiter (runs as quick as possible with a set limit)
// Must be more than the fixed update, or the sleep will disrupt the fixed timer
constexpr float c_DesiredRenderFPS = 60.0f;
constexpr float c_DesiredRenderFrameMS = c_MillisPerSecond / c_DesiredRenderFPS;

Application* Application::s_Instance = nullptr;

Application::Application()
    : m_World()
    , m_Window()
    , m_Running(true)
{
  BGE_CORE_ASSERT(!s_Instance, "Application already exists!");
  s_Instance = this;

  m_Window.Create(WindowData("BGE Window"));
  m_Window.SetEventCallback(BGE_BIND_EVENT_FN(Application::OnEvent));
  m_World.SetEventCallback(BGE_BIND_EVENT_FN(Application::OnEvent));

  Scheduler::Initialize();
  RenderDevice::Initialize();
  PhysicsDevice::Initialize();

  m_World.Init();
}

Application::~Application()
{
  RenderDevice::Shutdown();
  Scheduler::Shutdown();
}

void Application::Run()
{
  BGE_CORE_TRACE("Start running the application!.");

  Timer updateTimer;
  float millisElapsed = updateTimer.GetElapsedMilli();

  Timer renderTimer;

  while (m_Running)
  {
    int32 numUpdates = 0;

    while (updateTimer.GetElapsedMilli() > millisElapsed &&
           numUpdates < c_MaxUpdatesPerFrame)
    {
      millisElapsed += c_DesiredUpdateFrameMS;
      ++numUpdates;

      m_World.Update(c_FixedUpdateDeltaSeconds);
    }

    // Get how close we are to the next fixed update
    float interpolation = (updateTimer.GetElapsedMilli() +
                           c_DesiredUpdateFrameMS - millisElapsed) /
                          c_DesiredUpdateFrameMS;

    m_World.Render(interpolation);

    m_Window.OnTick();

    float elapsed = renderTimer.GetElapsedMilli();
    if (elapsed < c_DesiredRenderFrameMS)
    {
      std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(
          c_DesiredRenderFrameMS - elapsed));
    }
    renderTimer.Renew();

    // BGE_CORE_INFO("Frame completed in {0} milli", elapsed);
    // BGE_CORE_INFO("numUpdates: {0}", numUpdates);
    // BGE_CORE_INFO("interpolation: {0}", interpolation);
  }
}

void Application::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<WindowCloseEvent>(
      BGE_BIND_EVENT_FN(Application::OnWindowClose));

  m_World.OnEvent(event);
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
  BGE_CORE_INFO("Closing Window!.");

  m_Running = false;
  return true;
}

} // namespace bge