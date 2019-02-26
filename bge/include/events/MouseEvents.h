#pragma once

#include "Event.h"

namespace bge
{

class BGE_API MouseMovedEvent : public Event
{
public:
  MouseMovedEvent(float x, float y)
      : m_MouseX(x)
      , m_MouseY(y)
  {
  }

  FORCEINLINE float GetX() const { return m_MouseX; }
  FORCEINLINE float GetY() const { return m_MouseY; }

  EVENT_CLASS_TYPE(MouseMoved)

private:
  float m_MouseX, m_MouseY;
};

class BGE_API MouseScrolledEvent : public Event
{
public:
  MouseScrolledEvent(float xOffset, float yOffset)
      : m_XOffset(xOffset)
      , m_YOffset(yOffset)
  {
  }

  FORCEINLINE float GetXOffset() const { return m_XOffset; }
  FORCEINLINE float GetYOffset() const { return m_YOffset; }

  EVENT_CLASS_TYPE(MouseScrolled)

private:
  float m_XOffset, m_YOffset;
};

class BGE_API MouseButtonEvent : public Event
{
public:
  FORCEINLINE int32 GetMouseButton() const { return m_Button; }

protected:
  MouseButtonEvent(int32 button)
      : m_Button(button)
  {
  }

  int32 m_Button;
};

class BGE_API MouseButtonPressedEvent : public MouseButtonEvent
{
public:
  MouseButtonPressedEvent(int32 button)
      : MouseButtonEvent(button)
  {
  }

  EVENT_CLASS_TYPE(MouseButtonPressed)
};

class BGE_API MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
  MouseButtonReleasedEvent(int32 button)
      : MouseButtonEvent(button)
  {
  }

  EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace bge