#include "scheduler/WorkStealingQueue.h"

namespace bge
{

WorkStealingQueue::WorkStealingQueue()
    : m_Tasks()
    , m_Mutex()
    , m_Bottom(0u)
    , m_Top(0u)
{
}

void WorkStealingQueue::Push(Task* task)
{
  std::lock_guard<std::mutex> lock(m_Mutex);

  m_Tasks[m_Bottom & c_Mask] = task;
  ++m_Bottom;
}

Task* WorkStealingQueue::Pop()
{
  std::lock_guard<std::mutex> lock(m_Mutex);

  const int taskCount = m_Bottom - m_Top;

  if (taskCount <= 0)
  {
    // No tasks in queue
    return nullptr;
  }

  --m_Bottom;
  return m_Tasks[m_Bottom & c_Mask];
}

Task* WorkStealingQueue::Steal()
{
  std::lock_guard<std::mutex> lock(m_Mutex);

  const int taskCount = m_Bottom - m_Top;

  if (taskCount <= 0)
  {
    // No tasks in queue
    return nullptr;
  }

  Task* task = m_Tasks[m_Top & c_Mask];
  ++m_Top;
  return task;
}

} // namespace bge