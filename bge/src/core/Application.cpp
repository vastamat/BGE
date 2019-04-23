#include "core/Application.h"

#include "logging/Log.h"
#include "rendering/RenderDevice.h"

#include <functional>
#include <thread>

namespace bge
{

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

  RenderDevice::Initialize();
}

Application::~Application() { RenderDevice::Shutdown(); }

void Application::Run()
{
  BGE_CORE_TRACE("Start running the application!.");

  while (m_Running)
  {

    m_World.Update(1.0f);
    m_World.Render(1.0f);

    m_Window.OnTick();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
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