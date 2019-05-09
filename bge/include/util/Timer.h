#pragma once

#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

namespace bge
{

/////////////////////////////////////////////////
/// The timer begins running once constructer and can be queried for elapsed
/// time or restarted.
/////////////////////////////////////////////////
class Timer
{
public:
  Timer();

  void Renew();

  float GetElapsedMilli() const;
  float GetElapsedSeconds() const;

private:
  TimePoint m_Start;
};

} // namespace bge