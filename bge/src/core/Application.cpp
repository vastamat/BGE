#include "core/Application.h"

#include "logging/Log.h"

#include <functional>
#include <thread>

namespace bge
{

Application* Application::s_Instance = nullptr;

Application::Application()
    : m_Window()
    , m_Running(true)
{
  BGE_CORE_ASSERT(!s_Instance, "Application already exists!");
  s_Instance = this;

  m_Window.Create(WindowData("BGE"));
  m_Window.SetEventCallback(BGE_BIND_EVENT_FN(Application::OnEvent));
}

Application::~Application() {}

void Application::Run()
{
  BGE_CORE_TRACE("Start running the application!.");

  while (m_Running)
  {
    m_Window.OnTick();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}

void Application::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);

  dispatcher.Dispatch<WindowCloseEvent>(
      BGE_BIND_EVENT_FN(Application::OnWindowClose));
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
  BGE_CORE_INFO("Closing Window!.");

  m_Running = false;
  return true;
}

} // namespace bge