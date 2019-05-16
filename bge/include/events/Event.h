#pragma once

#include "core/Common.h"

#include <functional>

namespace bge
{

/**
 * All existing event types
 */
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
  MouseScrolled,
  ComponentAdded,
  ComponentRemoved,
  EntitiesDestroyed,
  CollidedBodies
};

#define EVENT_CLASS_TYPE(type)                                                 \
  static EventType GetStaticType() { return EventType::type; }                 \
  virtual EventType GetEventType() const override { return GetStaticType(); }

/**
 * Base class for all events.
 * Derived classes must override the GetEventType function,
 * and implement a "static EventType GetStaticType() {}"
 * which is needed to query the type of the event in the dispatcher.
 * The macro above can be used for an easy way to do that.
 */
class Event
{
public:
  bool m_Handled = false; /**< flag whether the event has been handled */

  virtual EventType GetEventType() const = 0;
};

/**
 * The event dispatcher handles the dispatching of events by
 * invoking the passed handler function only of the event types match
 */
class EventDispatcher
{
  template <typename T> using EventFn = std::function<bool(T&)>;

public:
  explicit EventDispatcher(Event& event)
      : m_Event(event)
  {
  }

  /**
   * The dispatch function is used to call event handler function safely
   * by ignoring ones which do not match the event type
   * @param func the event handler function of a specific event type
   * @return flag whether the event has been dispatched successfully
   */
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
  Event& m_Event; /**< Event which is being dispatched */
};

} // namespace bge