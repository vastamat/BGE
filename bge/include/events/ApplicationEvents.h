#pragma once

#include "Event.h"

namespace bge
{

class BGE_API WindowResizeEvent : public Event
{
public:
  WindowResizeEvent(uint32 width, uint32 height)
      : m_Width(width)
      , m_Height(height)
  {
  }

  FORCEINLINE uint32 GetWidth() const { return m_Width; }
  FORCEINLINE uint32 GetHeight() const { return m_Height; }

  EVENT_CLASS_TYPE(WindowResize)

private:
  uint32 m_Width, m_Height;
};

class BGE_API WindowCloseEvent : public Event
{
public:
  WindowCloseEvent() {}

  EVENT_CLASS_TYPE(WindowClose)
};

}