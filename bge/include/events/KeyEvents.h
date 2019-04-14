#pragma once

#include "Event.h"
#include "input/KeyCodes.h"

namespace bge
{

class KeyEvent : public Event
{
public:
  FORCEINLINE KeyCode GetKeyCode() const noexcept { return m_KeyCode; }

protected:
  KeyEvent(KeyCode keycode)
      : m_KeyCode(keycode)
  {
  }

  KeyCode m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
  KeyPressedEvent(KeyCode keycode, int32 repeatCount)
      : KeyEvent(keycode)
      , m_RepeatCount(repeatCount)
  {
  }

  FORCEINLINE int GetRepeatCount() const noexcept { return m_RepeatCount; }

  EVENT_CLASS_TYPE(KeyPressed)

private:
  int32 m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
  KeyReleasedEvent(KeyCode keycode)
      : KeyEvent(keycode)
  {
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
  KeyTypedEvent(KeyCode keycode)
      : KeyEvent(keycode)
  {
  }

  EVENT_CLASS_TYPE(KeyTyped)
};

} // namespace bge