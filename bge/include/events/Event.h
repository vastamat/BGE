#pragma once

#include "core/Common.h"

#include <functional>

namespace bge
{

////////////////////////////////////////////////////////////////
// All existing event types
///////////////////////////////////////////////////////////////
enum class EventType
{
  None = 0,
  WindowClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

// enum EventCategory
// {
//   None = 0,
//   EventCategoryApplication = 1 << 0,
//   EventCategoryInput = 1 << 1,
//   EventCategoryKeyboard = 1 << 2,
//   EventCategoryMouse = 1 << 3,
//   EventCategoryMouseButton = 1 << 4
// };

#define EVENT_CLASS_TYPE(type)                                                 \
  static EventType GetStaticType() { return EventType::type; }                 \
  virtual EventType GetEventType() const override { return GetStaticType(); }

////////////////////////////////////////////////////////////////
// Base class for all events.
// Derived classes must override the GetEventType function,
// and implement a "static EventType GetStaticType() {}"
// which is needed to query the type of the event in the dispatcher.
// The macro above can be used for an easy way to do that.
///////////////////////////////////////////////////////////////
class BGE_API Event
{
public:
  bool m_Handled = false;

  virtual EventType GetEventType() const = 0;
};

class EventDispatcher
{
  template <typename T> using EventFn = std::function<bool(T&)>;

public:
  explicit EventDispatcher(Event& event)
      : m_Event(event)
  {
  }

  template <typename T> bool Dispatch(EventFn<T> func)
  {
    if (m_Event.GetEventType() == T::GetStaticType())
    {
      m_Event.m_Handled = func(*(T*)&m_Event);
      return true;
    }
    return false;
  }

private:
  Event& m_Event;
};

} // namespace bge