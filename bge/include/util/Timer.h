#pragma once

#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

namespace bge
{

/**
 * The timer begins running once constructed and can be queried for elapsed
 * time or restarted.
 */
class Timer
{
public:
  Timer();

  /**
   * Renew the timer.
   */
  void Renew();

  /**
   * Fetch how many millis have passed since creation or renewal.
   * @return millis elapsed count
   */
  float GetElapsedMilli() const;

  /**
   * Fetch how many seconds have passed since creation or renewal.
   * @return seconds elapsed count
   */
  float GetElapsedSeconds() const;

private:
  TimePoint m_Start; /**< The starting time point */
};

} // namespace bge