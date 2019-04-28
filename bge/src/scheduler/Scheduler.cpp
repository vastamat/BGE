#include "scheduler/Scheduler.h"

#include "scheduler/WorkStealingQueue.h"
#include "util/RandomNumberGenerator.h"
#include <logging/Log.h>

#include <thread>
#include <unordered_map>
#include <vector>

namespace bge
{
namespace Scheduler
{

/// number of tasks the ring buffer stores
static constexpr int32 c_MaxTaskCount = 1024;

/// ring buffer of tasks for each thread
static thread_local Task s_TaskAllocator[c_MaxTaskCount];

/// counter to keep count of the number of total allocated tasks
static thread_local uint32 s_AllocatedTasksCount = 0u;

/// worker threads
static std::vector<std::unique_ptr<std::thread>> s_Threads;
/// work stealing queues per thread
static std::vector<std::unique_ptr<WorkStealingQueue>> s_TaskQueues;

/// thread id to array index
static std::unordered_map<std::thread::id, uint32> s_ThreadIdToIndex;

/// shutting down flag
static std::atomic_bool s_IsShuttingDown;
///< worker thread count
static uint32 s_WorkerThreadCount = 0u;
static RandomNumberGenerator s_RNG;

WorkStealingQueue* GetWorkerThreadQueue()
{
  auto threadId = std::this_thread::get_id();
  auto index = s_ThreadIdToIndex[threadId];
  return s_TaskQueues[index].get();
}

Task* AllocateTask()
{
  // Get a task specific for a thread (uses only thread-local vars)
  const uint32 index = s_AllocatedTasksCount++;

  // Warning: if more than c_MaxTaskCount tasks are allocated in 1 frame, then
  // it's possible that the first task may not have finished and will be
  // overwritten.
  return &s_TaskAllocator[index & (c_MaxTaskCount - 1)];
}

bool HasTaskCompleted(const Task* task) { return task->m_UnfinishedTasks == 0; }

Task* GetTask()
{
  WorkStealingQueue* queue = GetWorkerThreadQueue();

  if (queue == nullptr)
  {
    return nullptr;
  }

  Task* task = queue->Pop();

  // Check to see if a task was found
  if (task == nullptr)
  {
    // Task queue empty, try stealing from some other queue

    // Get a task queue index [0; threadCount]. 0 to threadCount includes the
    // main thread, because the max number is included (eg 0 to 7 for 8 threads)
    uint32 randomIndex = s_RNG.GenRandInt<uint32>(0, s_TaskQueues.size() - 1);

    WorkStealingQueue* stealQueue = s_TaskQueues[randomIndex].get();

    if (stealQueue == queue)
    {
      // don't try to steal from ourselves
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      return nullptr;
    }

    // Try to steal a job from the random queue
    Task* stolenTask = stealQueue->Steal();

    if (stolenTask == nullptr)
    {
      // we couldn't steal a job from the other queue either, so we just yield
      // our time slice for now
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      return nullptr;
    }

    return stolenTask;
  }

  return task;
}

void Finish(Task* task)
{
  --task->m_UnfinishedTasks;

  assert(task->m_UnfinishedTasks >= 0);

  if ((task->m_UnfinishedTasks == 0) && (task->m_Parent))
  {
    Finish(task->m_Parent);
  }
}

void Execute(Task* task)
{
  (task->m_Function)(task, task->m_Data);
  Finish(task);
}

void WorkerThreadMain()
{
  while (!s_IsShuttingDown)
  {
    Task* task = GetTask();
    if (task)
    {
      Execute(task);
    }
  }
}

void Initialize()
{
  s_IsShuttingDown.store(false);

  // Fetch the number of supported threads and subtract 1, which is the main
  // thread.
  s_WorkerThreadCount = std::thread::hardware_concurrency() - 1;
  s_Threads.reserve(s_WorkerThreadCount);
  s_TaskQueues.reserve(s_WorkerThreadCount + 1);
  s_ThreadIdToIndex.reserve(s_WorkerThreadCount + 1);

  // Add the worker queue for the main thread
  s_TaskQueues.emplace_back(std::make_unique<WorkStealingQueue>());
  s_ThreadIdToIndex[std::this_thread::get_id()] = 0;

  // Instantiate threads count - 1, because the main thread is already running
  for (size_t i = 0; i < s_WorkerThreadCount; i++)
  {
    s_Threads.emplace_back(std::make_unique<std::thread>(WorkerThreadMain));
    s_TaskQueues.emplace_back(std::make_unique<WorkStealingQueue>());
    s_ThreadIdToIndex[s_Threads.back()->get_id()] = i + 1;
  }
}

void Shutdown()
{
  // set shutdown to true
  s_IsShuttingDown.store(true);

  // join the threads to not crash
  for (auto&& thread : s_Threads)
  {
    thread->join();
  }
}

Task* CreateTask(TaskFunction function)
{
  Task* task = AllocateTask();

  task->m_Function = function;
  task->m_Parent = nullptr;
  task->m_UnfinishedTasks = 1;

  return task;
}

Task* CreateTask(TaskFunction function, const void* taskData,
                 size_t taskDataSize)
{
  BGE_CORE_ASSERT(taskDataSize <= c_SpaceForTaskData,
                  "Not enough space to store task data");

  Task* task = AllocateTask();

  task->m_Function = function;
  task->m_Parent = nullptr;
  task->m_UnfinishedTasks = 1;

  std::memcpy(task->m_Data, taskData, taskDataSize);

  return task;
}

Task* CreateChildTask(Task* parent, TaskFunction function)
{
  ++parent->m_UnfinishedTasks;

  Task* task = AllocateTask();

  task->m_Function = function;
  task->m_Parent = parent;
  task->m_UnfinishedTasks = 1;

  return task;
}

Task* CreateChildTask(Task* parent, TaskFunction function, const void* taskData,
                      size_t taskDataSize)
{
  BGE_CORE_ASSERT(taskDataSize <= c_SpaceForTaskData,
                  "Not enough space to store task data");

  ++parent->m_UnfinishedTasks;

  Task* task = AllocateTask();

  task->m_Function = function;
  task->m_Parent = parent;
  task->m_UnfinishedTasks = 1;

  std::memcpy(task->m_Data, taskData, taskDataSize);

  return task;
}

void Run(Task* task)
{
  if (WorkStealingQueue* queue = GetWorkerThreadQueue())
  {
    queue->Push(task);
  }
}

void Wait(const Task* task)
{
  // wait until the job has completed. in the meantime, work on any other job.
  while (!HasTaskCompleted(task))
  {
    Task* nextTask = GetTask();
    if (nextTask)
    {
      Execute(nextTask);
    }
  }
}

void EmptyTask(Task* task, const void* taskData) {}

} // namespace Scheduler
} // namespace bge