#if defined BGE_PLATFORM_UNIX || BGE_PLATFORM_APPLE || BGE_PLATFORM_WINDOWS

#include "input/Input.h"

#include "core/Application.h"

#include "GLFW/glfw3.h"

namespace bge
{

KeyCode GetKeyCodeFromGLFWKey(int key)
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

MouseButtonCode GetMouseButtonCodeFromGLFWButton(int button)
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