#if defined BGE_PLATFORM_UNIX || BGE_PLATFORM_APPLE || BGE_PLATFORM_WINDOWS

#include "input/Input.h"

#include "core/Application.h"

#include <GLFW/glfw3.h>

namespace bge
{

int GetGLFWKeyFromKeyCode(KeyCode key)
{
  switch (key)
  {
    case KeyCode::Unknown:
      return GLFW_KEY_UNKNOWN;
    case KeyCode::Backspace:
      return GLFW_KEY_BACKSPACE;
    case KeyCode::Tab:
      return GLFW_KEY_TAB;
    case KeyCode::Return:
      return GLFW_KEY_ENTER;
    case KeyCode::Pause:
      return GLFW_KEY_PAUSE;
    case KeyCode::Escape:
      return GLFW_KEY_ESCAPE;
    case KeyCode::Space:
      return GLFW_KEY_SPACE;
    case KeyCode::Comma:
      return GLFW_KEY_COMMA;
    case KeyCode::Minus:
      return GLFW_KEY_MINUS;
    case KeyCode::Period:
      return GLFW_KEY_PERIOD;
    case KeyCode::Slash:
      return GLFW_KEY_SLASH;
    case KeyCode::Zero:
      return GLFW_KEY_0;
    case KeyCode::One:
      return GLFW_KEY_1;
    case KeyCode::Two:
      return GLFW_KEY_2;
    case KeyCode::Three:
      return GLFW_KEY_3;
    case KeyCode::Four:
      return GLFW_KEY_4;
    case KeyCode::Five:
      return GLFW_KEY_5;
    case KeyCode::Six:
      return GLFW_KEY_6;
    case KeyCode::Seven:
      return GLFW_KEY_7;
    case KeyCode::Eight:
      return GLFW_KEY_8;
    case KeyCode::Nine:
      return GLFW_KEY_9;
    case KeyCode::Semicolon:
      return GLFW_KEY_SEMICOLON;
    case KeyCode::Equals:
      return GLFW_KEY_EQUAL;
    case KeyCode::LeftBracket:
      return GLFW_KEY_LEFT_BRACKET;
    case KeyCode::Backslash:
      return GLFW_KEY_BACKSLASH;
    case KeyCode::RightBracket:
      return GLFW_KEY_RIGHT_BRACKET;
    case KeyCode::A:
      return GLFW_KEY_A;
    case KeyCode::B:
      return GLFW_KEY_B;
    case KeyCode::C:
      return GLFW_KEY_C;
    case KeyCode::D:
      return GLFW_KEY_D;
    case KeyCode::E:
      return GLFW_KEY_E;
    case KeyCode::F:
      return GLFW_KEY_F;
    case KeyCode::G:
      return GLFW_KEY_G;
    case KeyCode::H:
      return GLFW_KEY_H;
    case KeyCode::I:
      return GLFW_KEY_I;
    case KeyCode::J:
      return GLFW_KEY_J;
    case KeyCode::K:
      return GLFW_KEY_K;
    case KeyCode::L:
      return GLFW_KEY_L;
    case KeyCode::M:
      return GLFW_KEY_M;
    case KeyCode::N:
      return GLFW_KEY_N;
    case KeyCode::O:
      return GLFW_KEY_O;
    case KeyCode::P:
      return GLFW_KEY_P;
    case KeyCode::Q:
      return GLFW_KEY_Q;
    case KeyCode::R:
      return GLFW_KEY_R;
    case KeyCode::S:
      return GLFW_KEY_S;
    case KeyCode::T:
      return GLFW_KEY_T;
    case KeyCode::U:
      return GLFW_KEY_U;
    case KeyCode::V:
      return GLFW_KEY_V;
    case KeyCode::W:
      return GLFW_KEY_W;
    case KeyCode::X:
      return GLFW_KEY_X;
    case KeyCode::Y:
      return GLFW_KEY_Y;
    case KeyCode::Z:
      return GLFW_KEY_Z;
    case KeyCode::Delete:
      return GLFW_KEY_DELETE;
    case KeyCode::Decimal:
      return GLFW_KEY_KP_DECIMAL;
    case KeyCode::Divide:
      return GLFW_KEY_KP_DIVIDE;
    case KeyCode::Multiply:
      return GLFW_KEY_KP_MULTIPLY;
    case KeyCode::Subtract:
      return GLFW_KEY_KP_SUBTRACT;
    case KeyCode::Add:
      return GLFW_KEY_KP_ADD;
    case KeyCode::NumpadEnter:
      return GLFW_KEY_KP_ENTER;
    case KeyCode::NumpadEquals:
      return GLFW_KEY_KP_EQUAL;
    case KeyCode::ArrowUp:
      return GLFW_KEY_UP;
    case KeyCode::ArrowDown:
      return GLFW_KEY_DOWN;
    case KeyCode::ArrowRight:
      return GLFW_KEY_RIGHT;
    case KeyCode::ArrowLeft:
      return GLFW_KEY_LEFT;
    case KeyCode::Insert:
      return GLFW_KEY_INSERT;
    case KeyCode::Home:
      return GLFW_KEY_HOME;
    case KeyCode::End:
      return GLFW_KEY_END;
    case KeyCode::PageUp:
      return GLFW_KEY_PAGE_UP;
    case KeyCode::PageDown:
      return GLFW_KEY_PAGE_DOWN;
    case KeyCode::F1:
      return GLFW_KEY_F1;
    case KeyCode::F2:
      return GLFW_KEY_F2;
    case KeyCode::F3:
      return GLFW_KEY_F3;
    case KeyCode::F4:
      return GLFW_KEY_F4;
    case KeyCode::F5:
      return GLFW_KEY_F5;
    case KeyCode::F6:
      return GLFW_KEY_F6;
    case KeyCode::F7:
      return GLFW_KEY_F7;
    case KeyCode::F8:
      return GLFW_KEY_F8;
    case KeyCode::F9:
      return GLFW_KEY_F9;
    case KeyCode::F10:
      return GLFW_KEY_F10;
    case KeyCode::F11:
      return GLFW_KEY_F11;
    case KeyCode::F12:
      return GLFW_KEY_F12;
    case KeyCode::F13:
      return GLFW_KEY_F13;
    case KeyCode::F14:
      return GLFW_KEY_F14;
    case KeyCode::F15:
      return GLFW_KEY_F15;
    case KeyCode::RightShift:
      return GLFW_KEY_RIGHT_SHIFT;
    case KeyCode::LeftShift:
      return GLFW_KEY_LEFT_SHIFT;
    case KeyCode::RightControl:
      return GLFW_KEY_RIGHT_CONTROL;
    case KeyCode::LeftControl:
      return GLFW_KEY_LEFT_CONTROL;
    case KeyCode::RightAlt:
      return GLFW_KEY_RIGHT_ALT;
    case KeyCode::LeftAlt:
      return GLFW_KEY_LEFT_ALT;
    case KeyCode::AppMenu:
      return GLFW_KEY_MENU;
    case KeyCode::SysRq:
      return GLFW_KEY_PRINT_SCREEN;
  }
  return GLFW_KEY_UNKNOWN;
}

int GetGLFWButtonFromMouseButtonCode(MouseButtonCode button)
{
  switch (button)
  {
    case MouseButtonCode::Unknown:
      return GLFW_KEY_UNKNOWN;
    case MouseButtonCode::LeftMouseButton:
      return GLFW_MOUSE_BUTTON_LEFT;
    case MouseButtonCode::MiddleMouseButton:
      return GLFW_MOUSE_BUTTON_MIDDLE;
    case MouseButtonCode::RightMouseButton:
      return GLFW_MOUSE_BUTTON_RIGHT;
      return GLFW_MOUSE_BUTTON_3;
    case MouseButtonCode::MouseButton4:
      return GLFW_MOUSE_BUTTON_4;
    case MouseButtonCode::MouseButton5:
      return GLFW_MOUSE_BUTTON_5;
    case MouseButtonCode::MouseButton6:
      return GLFW_MOUSE_BUTTON_6;
    case MouseButtonCode::MouseButton7:
      return GLFW_MOUSE_BUTTON_7;
    case MouseButtonCode::MouseButton8:
      return GLFW_MOUSE_BUTTON_8;
  }

  return GLFW_KEY_UNKNOWN;
}

bool Input::IsKeyPressed(KeyCode key)
{
  GLFWwindow* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  int state = glfwGetKey(window, GetGLFWKeyFromKeyCode(key));
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::IsMouseButtonPressed(MouseButtonCode button)
{
  GLFWwindow* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  int state =
      glfwGetMouseButton(window, GetGLFWButtonFromMouseButtonCode(button));
  return state == GLFW_PRESS;
}
std::pair<float, float> Input::GetMousePosition()
{
  GLFWwindow* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  return {(float)xpos, (float)ypos};
}
float Input::GetMouseX() { return GetMousePosition().first; }
float Input::GetMouseY() { return GetMousePosition().second; }

} // namespace bge

#endif