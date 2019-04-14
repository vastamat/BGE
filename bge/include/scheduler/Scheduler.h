#pragma once

#include "Task.h"

#include "core/Common.h"

namespace bge
{
namespace Scheduler
{

/// Initialize the scheduler, called once on startup
void Initialize();
/// Shutdown the scheduler, called on app exit
void Shutdown();

/// create a task which takes on data
Task* CreateTask(TaskFunction function);
/// create a task which takes in input data
Task* CreateTask(TaskFunction function, const void* taskData,
                 size_t taskDataSize);

/// create a child task with no data
Task* CreateChildTask(Task* parent, TaskFunction function);
///< create a child task with data
Task* CreateChildTask(Task* parent, TaskFunction function, const void* taskData,
                      size_t taskDataSize);

/// Run a task, which adds it to the task queue,
/// enabling other workers to steal it
void Run(Task* task);
/// wait for a task to finish, and while waiting,
/// help with executing tasks
void Wait(const Task* task);

/// task function which does nothing
/// (useful for empty root tasks)
void EmptyTask(Task* task, const void* taskData);

} // namespace Scheduler
} // namespace bge