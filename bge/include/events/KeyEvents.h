#pragma once

#include "Event.h"
#include "input/KeyCodes.h"

namespace bge
{

class BGE_API KeyEvent : public Event
{
public:
  FORCEINLINE KeyCode GetKeyCode() const { return m_KeyCode; }

protected:
  KeyEvent(KeyCode keycode)
      : m_KeyCode(keycode)
  {
  }

  KeyCode m_KeyCode;
};

class BGE_API KeyPressedEvent : public KeyEvent
{
public:
  KeyPressedEvent(KeyCode keycode, int32 repeatCount)
      : KeyEvent(keycode)
      , m_RepeatCount(repeatCount)
  {
  }

  FORCEINLINE int GetRepeatCount() const { return m_RepeatCount; }

  EVENT_CLASS_TYPE(KeyPressed)

private:
  int32 m_RepeatCount;
};

class BGE_API KeyReleasedEvent : public KeyEvent
{
public:
  KeyReleasedEvent(KeyCode keycode)
      : KeyEvent(keycode)
  {
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class BGE_API KeyTypedEvent : public KeyEvent
{
public:
  KeyTypedEvent(KeyCode keycode)
      : KeyEvent(keycode)
  {
  }

  EVENT_CLASS_TYPE(KeyTyped)
};

} // namespace bge