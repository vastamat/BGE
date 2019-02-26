#if defined BGE_PLATFORM_UNIX || BGE_PLATFORM_APPLE || BGE_PLATFORM_WINDOWS

#include "video/Window.h"

#include "events/ApplicationEvents.h"
#include "events/KeyEvents.h"
#include "events/MouseEvents.h"
#include "logging/Log.h"

#include "GLFW/glfw3.h"

namespace bge
{

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
  BGE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

class Window::Impl
{
public:
  Impl()
      : m_WindowData()
      , m_EventCallback()
      , m_NativeWindow(nullptr)
  {
  }

  ~Impl() { Destroy(); }

  DELETE_COPY_AND_ASSIGN(Impl);

  void Create(const WindowData& data)
  {
    m_WindowData = data;

    BGE_CORE_INFO("Creating window {0} ({1}, {2}, {3})", m_WindowData.m_Title,
                  m_WindowData.m_Width, m_WindowData.m_Height,
                  m_WindowData.m_VSync);

    if (!s_GLFWInitialized)
    {
      int success = glfwInit();
      BGE_CORE_ASSERT(success, "Could not intialize GLFW!");
      glfwSetErrorCallback(GLFWErrorCallback);
      s_GLFWInitialized = true;
    }

    m_NativeWindow =
        glfwCreateWindow((int)m_WindowData.m_Width, (int)m_WindowData.m_Height,
                         m_WindowData.m_Title.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(m_NativeWindow);
    glfwSetWindowUserPointer(m_NativeWindow, this);

    SetVSync(m_WindowData.m_VSync);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(
        m_NativeWindow, [](GLFWwindow* window, int width, int height) {
          Window::Impl& impl = *(Window::Impl*)glfwGetWindowUserPointer(window);
          impl.m_WindowData.m_Width = width;
          impl.m_WindowData.m_Height = height;

          WindowResizeEvent event(width, height);
          impl.m_EventCallback(event);
        });

    glfwSetWindowCloseCallback(m_NativeWindow, [](GLFWwindow* window) {
      Window::Impl& impl = *(Window::Impl*)glfwGetWindowUserPointer(window);
      WindowCloseEvent event;
      impl.m_EventCallback(event);
    });

    glfwSetKeyCallback(m_NativeWindow, [](GLFWwindow* window, int key,
                                          int scancode, int action, int mods) {
      Window::Impl& impl = *(Window::Impl*)glfwGetWindowUserPointer(window);

      switch (action)
      {
        case GLFW_PRESS:
        {
          KeyPressedEvent event(key, 0);
          impl.m_EventCallback(event);
          break;
        }
        case GLFW_RELEASE:
        {
          KeyReleasedEvent event(key);
          impl.m_EventCallback(event);
          break;
        }
        case GLFW_REPEAT:
        {
          KeyPressedEvent event(key, 1);
          impl.m_EventCallback(event);
          break;
        }
      }
    });

    glfwSetCharCallback(
        m_NativeWindow, [](GLFWwindow* window, unsigned int keycode) {
          Window::Impl& impl = *(Window::Impl*)glfwGetWindowUserPointer(window);

          KeyTypedEvent event(keycode);
          impl.m_EventCallback(event);
        });

    glfwSetMouseButtonCallback(
        m_NativeWindow,
        [](GLFWwindow* window, int button, int action, int mods) {
          Window::Impl& impl = *(Window::Impl*)glfwGetWindowUserPointer(window);

          switch (action)
          {
            case GLFW_PRESS:
            {
              MouseButtonPressedEvent event(button);
              impl.m_EventCallback(event);
              break;
            }
            case GLFW_RELEASE:
            {
              MouseButtonReleasedEvent event(button);
              impl.m_EventCallback(event);
              break;
            }
          }
        });

    glfwSetScrollCallback(
        m_NativeWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
          Window::Impl& impl = *(Window::Impl*)glfwGetWindowUserPointer(window);

          MouseScrolledEvent event((float)xOffset, (float)yOffset);
          impl.m_EventCallback(event);
        });

    glfwSetCursorPosCallback(
        m_NativeWindow, [](GLFWwindow* window, double xPos, double yPos) {
          Window::Impl& impl = *(Window::Impl*)glfwGetWindowUserPointer(window);

          MouseMovedEvent event((float)xPos, (float)yPos);
          impl.m_EventCallback(event);
        });
  }

  void Destroy()
  {
    if (s_GLFWInitialized)
    {
      glfwDestroyWindow(m_NativeWindow);
      glfwTerminate();
      s_GLFWInitialized = false;
    }
  }

  // Tick called every frame
  void OnTick()
  {
    glfwPollEvents();
    glfwSwapBuffers(m_NativeWindow);
  }

  // Setters
  void SetEventCallback(const EventCallbackFn& callback)
  {
    m_EventCallback = callback;
  }
  void SetVSync(bool enabled)
  {
    if (enabled)
    {
      glfwSwapInterval(1);
    }
    else
    {
      glfwSwapInterval(0);
    }

    m_WindowData.m_VSync = enabled;
  }

  // Getters
  bool IsVSync() const { return m_WindowData.m_VSync; }
  uint32 GetWidth() const { return m_WindowData.m_Width; }
  uint32 GetHeight() const { return m_WindowData.m_Height; }
  void* GetNativeWindow() const { return (void*)m_NativeWindow; }

  WindowData m_WindowData;
  EventCallbackFn m_EventCallback;
  GLFWwindow* m_NativeWindow;
};

// ------------------------------------------------------------------

Window::Window()
    : m_impl(std::make_unique<Window::Impl>())
{
}

Window::~Window() { Destroy(); }

void Window::Create(const WindowData& data) { m_impl->Create(data); }
void Window::Destroy() { m_impl->Destroy(); }

// Tick called every frame
void Window::OnTick() { m_impl->OnTick(); }

// Setters
void Window::SetEventCallback(const EventCallbackFn& callback)
{
  m_impl->SetEventCallback(callback);
}
void Window::SetVSync(bool enabled) { m_impl->SetVSync(enabled); }

// Getters
bool Window::IsVSync() const { return m_impl->IsVSync(); }
uint32 Window::GetWidth() const { return m_impl->GetWidth(); }
uint32 Window::GetHeight() const { return m_impl->GetHeight(); }
void* Window::GetNativeWindow() const { return m_impl->GetNativeWindow(); }

} // namespace bge

#endif