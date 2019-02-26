#pragma once

#include "Event.h"

namespace bge
{

class BGE_API KeyEvent : public Event
{
public:
  FORCEINLINE int32 GetKeyCode() const { return m_KeyCode; }

protected:
  KeyEvent(int32 keycode)
      : m_KeyCode(keycode)
  {
  }

  int32 m_KeyCode;
};

class BGE_API KeyPressedEvent : public KeyEvent
{
public:
  KeyPressedEvent(int32 keycode, int32 repeatCount)
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
  KeyReleasedEvent(int32 keycode)
      : KeyEvent(keycode)
  {
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class BGE_API KeyTypedEvent : public KeyEvent
{
public:
  KeyTypedEvent(int keycode)
      : KeyEvent(keycode)
  {
  }

  EVENT_CLASS_TYPE(KeyTyped)
};

} // namespace bge