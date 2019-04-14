#pragma once

#include "Event.h"

namespace bge
{

class WindowResizeEvent : public Event
{
public:
  WindowResizeEvent(uint32 width, uint32 height)
      : m_Width(width)
      , m_Height(height)
  {
  }

  FORCEINLINE uint32 GetWidth() const noexcept { return m_Width; }
  FORCEINLINE uint32 GetHeight() const noexcept { return m_Height; }

  EVENT_CLASS_TYPE(WindowResize)

private:
  uint32 m_Width, m_Height;
};

class WindowCloseEvent : public Event
{
public:
  WindowCloseEvent() {}

  EVENT_CLASS_TYPE(WindowClose)
};

} // namespace bge