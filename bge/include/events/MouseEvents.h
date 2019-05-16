#pragma once

#include "Event.h"
#include "input/MouseButtonCodes.h"

namespace bge
{

/**
 * Event which represents the movement of the mouse
 */
class MouseMovedEvent : public Event
{
public:
  MouseMovedEvent(float x, float y)
      : m_MouseX(x)
      , m_MouseY(y)
  {
  }

  FORCEINLINE float GetX() const noexcept { return m_MouseX; }
  FORCEINLINE float GetY() const noexcept { return m_MouseY; }

  EVENT_CLASS_TYPE(MouseMoved)

private:
  float m_MouseX, m_MouseY; /**< the new x and y window coordinates */
};

/**
 * Event which represents the scrolling of the mouse scroll
 */
class MouseScrolledEvent : public Event
{
public:
  MouseScrolledEvent(float xOffset, float yOffset)
      : m_XOffset(xOffset)
      , m_YOffset(yOffset)
  {
  }

  FORCEINLINE float GetXOffset() const noexcept { return m_XOffset; }
  FORCEINLINE float GetYOffset() const noexcept { return m_YOffset; }

  EVENT_CLASS_TYPE(MouseScrolled)

private:
  float m_XOffset, m_YOffset; /**< the new x and y scroll offsets */
};

/**
 * Base class for mouse button events
 */
class MouseButtonEvent : public Event
{
public:
  FORCEINLINE MouseButtonCode GetMouseButton() const noexcept
  {
    return m_Button;
  }

protected:
  MouseButtonEvent(MouseButtonCode button)
      : m_Button(button)
  {
  }

  MouseButtonCode m_Button; /**< the button code of the button */
};

/**
 * Event which represents the pressing of a mouse button
 */
class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
  MouseButtonPressedEvent(MouseButtonCode button)
      : MouseButtonEvent(button)
  {
  }

  EVENT_CLASS_TYPE(MouseButtonPressed)
};

/**
 * Event which represents the releasing of a mouse button
 */
class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
  MouseButtonReleasedEvent(MouseButtonCode button)
      : MouseButtonEvent(button)
  {
  }

  EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace bge