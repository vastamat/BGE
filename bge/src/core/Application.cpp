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

constexpr int32 c_DesiredFPS = 25;
constexpr int32 c_MillisPerSecond = 1000;
constexpr int32 c_DesiredFrameTimeMS = c_MillisPerSecond / c_DesiredFPS;
constexpr int32 c_MaxUpdatesPerFrame = 5;
constexpr float c_FixedDeltaSeconds = c_DesiredFrameTimeMS / 1000.0f;

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

  Timer timer;
  float millisElapsed = timer.GetElapsedMilli();

  while (m_Running)
  {
    std::int32_t numUpdates = 0;

    while (timer.GetElapsedMilli() > millisElapsed &&
           numUpdates < c_MaxUpdatesPerFrame)
    {
      millisElapsed += c_DesiredFrameTimeMS;
      ++numUpdates;

      m_World.Update(c_FixedDeltaSeconds);
    }

    float interpolation =
        (timer.GetElapsedMilli() + c_DesiredFrameTimeMS - millisElapsed) /
        c_DesiredFrameTimeMS;

    m_World.Render(interpolation);

    m_Window.OnTick();
    // std::this_thread::sleep_for(std::chrono::milliseconds(16));
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