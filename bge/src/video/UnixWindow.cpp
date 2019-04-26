#if defined BGE_PLATFORM_UNIX || BGE_PLATFORM_APPLE || BGE_PLATFORM_WINDOWS

#include "video/Window.h"

#include "events/ApplicationEvents.h"
#include "events/KeyEvents.h"
#include "events/MouseEvents.h"
#include "logging/Log.h"

#include <GLFW/glfw3.h>

namespace bge
{

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
  BGE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

static KeyCode GetKeyCodeFromGLFWKey(int key)
{
  switch (key)
  {
    case GLFW_KEY_UNKNOWN:
      return KeyCode::Unknown;
    case GLFW_KEY_BACKSPACE:
      return KeyCode::Backspace;
    case GLFW_KEY_TAB:
      return KeyCode::Tab;
    case GLFW_KEY_ENTER:
      return KeyCode::Return;
    case GLFW_KEY_PAUSE:
      return KeyCode::Pause;
    case GLFW_KEY_ESCAPE:
      return KeyCode::Escape;
    case GLFW_KEY_SPACE:
      return KeyCode::Space;
    case GLFW_KEY_COMMA:
      return KeyCode::Comma;
    case GLFW_KEY_MINUS:
      return KeyCode::Minus;
    case GLFW_KEY_PERIOD:
      return KeyCode::Period;
    case GLFW_KEY_SLASH:
      return KeyCode::Slash;
    case GLFW_KEY_0:
      return KeyCode::Zero;
    case GLFW_KEY_1:
      return KeyCode::One;
    case GLFW_KEY_2:
      return KeyCode::Two;
    case GLFW_KEY_3:
      return KeyCode::Three;
    case GLFW_KEY_4:
      return KeyCode::Four;
    case GLFW_KEY_5:
      return KeyCode::Five;
    case GLFW_KEY_6:
      return KeyCode::Six;
    case GLFW_KEY_7:
      return KeyCode::Seven;
    case GLFW_KEY_8:
      return KeyCode::Eight;
    case GLFW_KEY_9:
      return KeyCode::Nine;
    case GLFW_KEY_SEMICOLON:
      return KeyCode::Semicolon;
    case GLFW_KEY_EQUAL:
      return KeyCode::Equals;
    case GLFW_KEY_LEFT_BRACKET:
      return KeyCode::LeftBracket;
    case GLFW_KEY_BACKSLASH:
      return KeyCode::Backslash;
    case GLFW_KEY_RIGHT_BRACKET:
      return KeyCode::RightBracket;
    case GLFW_KEY_A:
      return KeyCode::A;
    case GLFW_KEY_B:
      return KeyCode::B;
    case GLFW_KEY_C:
      return KeyCode::C;
    case GLFW_KEY_D:
      return KeyCode::D;
    case GLFW_KEY_E:
      return KeyCode::E;
    case GLFW_KEY_F:
      return KeyCode::F;
    case GLFW_KEY_G:
      return KeyCode::G;
    case GLFW_KEY_H:
      return KeyCode::H;
    case GLFW_KEY_I:
      return KeyCode::I;
    case GLFW_KEY_J:
      return KeyCode::J;
    case GLFW_KEY_K:
      return KeyCode::K;
    case GLFW_KEY_L:
      return KeyCode::L;
    case GLFW_KEY_M:
      return KeyCode::M;
    case GLFW_KEY_N:
      return KeyCode::N;
    case GLFW_KEY_O:
      return KeyCode::O;
    case GLFW_KEY_P:
      return KeyCode::P;
    case GLFW_KEY_Q:
      return KeyCode::Q;
    case GLFW_KEY_R:
      return KeyCode::R;
    case GLFW_KEY_S:
      return KeyCode::S;
    case GLFW_KEY_T:
      return KeyCode::T;
    case GLFW_KEY_U:
      return KeyCode::U;
    case GLFW_KEY_V:
      return KeyCode::V;
    case GLFW_KEY_W:
      return KeyCode::W;
    case GLFW_KEY_X:
      return KeyCode::X;
    case GLFW_KEY_Y:
      return KeyCode::Y;
    case GLFW_KEY_Z:
      return KeyCode::Z;
    case GLFW_KEY_DELETE:
      return KeyCode::Delete;
    case GLFW_KEY_KP_DECIMAL:
      return KeyCode::Decimal;
    case GLFW_KEY_KP_DIVIDE:
      return KeyCode::Divide;
    case GLFW_KEY_KP_MULTIPLY:
      return KeyCode::Multiply;
    case GLFW_KEY_KP_SUBTRACT:
      return KeyCode::Subtract;
    case GLFW_KEY_KP_ADD:
      return KeyCode::Add;
    case GLFW_KEY_KP_ENTER:
      return KeyCode::NumpadEnter;
    case GLFW_KEY_KP_EQUAL:
      return KeyCode::NumpadEquals;
    case GLFW_KEY_UP:
      return KeyCode::ArrowUp;
    case GLFW_KEY_DOWN:
      return KeyCode::ArrowDown;
    case GLFW_KEY_RIGHT:
      return KeyCode::ArrowRight;
    case GLFW_KEY_LEFT:
      return KeyCode::ArrowLeft;
    case GLFW_KEY_INSERT:
      return KeyCode::Insert;
    case GLFW_KEY_HOME:
      return KeyCode::Home;
    case GLFW_KEY_END:
      return KeyCode::End;
    case GLFW_KEY_PAGE_UP:
      return KeyCode::PageUp;
    case GLFW_KEY_PAGE_DOWN:
      return KeyCode::PageDown;
    case GLFW_KEY_F1:
      return KeyCode::F1;
    case GLFW_KEY_F2:
      return KeyCode::F2;
    case GLFW_KEY_F3:
      return KeyCode::F3;
    case GLFW_KEY_F4:
      return KeyCode::F4;
    case GLFW_KEY_F5:
      return KeyCode::F5;
    case GLFW_KEY_F6:
      return KeyCode::F6;
    case GLFW_KEY_F7:
      return KeyCode::F7;
    case GLFW_KEY_F8:
      return KeyCode::F8;
    case GLFW_KEY_F9:
      return KeyCode::F9;
    case GLFW_KEY_F10:
      return KeyCode::F10;
    case GLFW_KEY_F11:
      return KeyCode::F11;
    case GLFW_KEY_F12:
      return KeyCode::F12;
    case GLFW_KEY_F13:
      return KeyCode::F13;
    case GLFW_KEY_F14:
      return KeyCode::F14;
    case GLFW_KEY_F15:
      return KeyCode::F15;
    case GLFW_KEY_RIGHT_SHIFT:
      return KeyCode::RightShift;
    case GLFW_KEY_LEFT_SHIFT:
      return KeyCode::LeftShift;
    case GLFW_KEY_RIGHT_CONTROL:
      return KeyCode::RightControl;
    case GLFW_KEY_LEFT_CONTROL:
      return KeyCode::LeftControl;
    case GLFW_KEY_RIGHT_ALT:
      return KeyCode::RightAlt;
    case GLFW_KEY_LEFT_ALT:
      return KeyCode::LeftAlt;
    case GLFW_KEY_MENU:
      return KeyCode::AppMenu;
    case GLFW_KEY_PRINT_SCREEN:
      return KeyCode::SysRq;
  }
  return KeyCode::Unknown;
}

static MouseButtonCode GetMouseButtonCodeFromGLFWButton(int button)
{
  switch (button)
  {
    case GLFW_MOUSE_BUTTON_LEFT:
      return MouseButtonCode::LeftMouseButton;
    case GLFW_MOUSE_BUTTON_MIDDLE:
      return MouseButtonCode::MiddleMouseButton;
    case GLFW_MOUSE_BUTTON_RIGHT:
      return MouseButtonCode::RightMouseButton;
    case GLFW_MOUSE_BUTTON_4:
      return MouseButtonCode::MouseButton4;
    case GLFW_MOUSE_BUTTON_5:
      return MouseButtonCode::MouseButton5;
    case GLFW_MOUSE_BUTTON_6:
      return MouseButtonCode::MouseButton6;
    case GLFW_MOUSE_BUTTON_7:
      return MouseButtonCode::MouseButton7;
    case GLFW_MOUSE_BUTTON_8:
      return MouseButtonCode::MouseButton8;
  }

  return MouseButtonCode::Unknown;
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
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
          KeyPressedEvent event(GetKeyCodeFromGLFWKey(key), 0);
          impl.m_EventCallback(event);
          break;
        }
        case GLFW_RELEASE:
        {
          KeyReleasedEvent event(GetKeyCodeFromGLFWKey(key));
          impl.m_EventCallback(event);
          break;
        }
        case GLFW_REPEAT:
        {
          KeyPressedEvent event(GetKeyCodeFromGLFWKey(key), 1);
          impl.m_EventCallback(event);
          break;
        }
      }
    });

    glfwSetCharCallback(
        m_NativeWindow, [](GLFWwindow* window, unsigned int keycode) {
          Window::Impl& impl = *(Window::Impl*)glfwGetWindowUserPointer(window);

          KeyTypedEvent event(GetKeyCodeFromGLFWKey(keycode));
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
              MouseButtonPressedEvent event(
                  GetMouseButtonCodeFromGLFWButton(button));
              impl.m_EventCallback(event);
              break;
            }
            case GLFW_RELEASE:
            {
              MouseButtonReleasedEvent event(
                  GetMouseButtonCodeFromGLFWButton(button));
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
  void SetCursor(bool enabled)
  {
    int mode = GLFW_CURSOR_NORMAL;

    if (!enabled)
    {
      mode = GLFW_CURSOR_DISABLED;
    }

    glfwSetInputMode(m_NativeWindow, GLFW_CURSOR, mode);
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
void Window::SetCursor(bool enabled) { m_impl->SetCursor(enabled); }

// Getters
bool Window::IsVSync() const { return m_impl->IsVSync(); }
uint32 Window::GetWidth() const { return m_impl->GetWidth(); }
uint32 Window::GetHeight() const { return m_impl->GetHeight(); }
void* Window::GetNativeWindow() const { return m_impl->GetNativeWindow(); }

} // namespace bge

#endif