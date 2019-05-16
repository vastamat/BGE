#pragma once

#include "Event.h"

namespace bge
{

/**
 * Event for when the app window is resized
 */
class WindowResizeEvent : public Event
{
public:
  WindowResizeEvent(uint32 width, uint32 height)
      : m_Width(width)
      , m_Height(height)
  {
  }

  /**
   * @return the new width of the window
   */
  FORCEINLINE uint32 GetWidth() const noexcept { return m_Width; }

  /**
   * @return the new height of the window
   */
  FORCEINLINE uint32 GetHeight() const noexcept { return m_Height; }

  EVENT_CLASS_TYPE(WindowResize)

private:
  uint32 m_Width, m_Height; /**< The new width and height of the window */
};

/**
 * Event for when the app window is closed
 */
class WindowCloseEvent : public Event
{
public:
  WindowCloseEvent() {}

  EVENT_CLASS_TYPE(WindowClose)
};

} // namespace bge