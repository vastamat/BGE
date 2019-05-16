#pragma once

#include "Task.h"

#include "core/Common.h"

namespace bge
{
namespace Scheduler
{

/**
 * Initialize the scheduler, called once on startup
 */
void Initialize();

/**
 * Shutdown the scheduler, called on app exit
 */
void Shutdown();

/**
 * create a task which takes no data
 * @param function the task function to execute
 * @return pointer to the created task
 */
Task* CreateTask(TaskFunction function);

/**
 * create a task which takes in input data
 * @param function the task function to execute
 * @param taskData pointer to input data for the task
 * @param taskDataSize the size of the memory block the pointer points to
 * @return pointer to the created task
 */
Task* CreateTask(TaskFunction function, const void* taskData,
                 size_t taskDataSize);

/**
 * create a child task with no data
 * @param parent pointer to the parent of this task
 * @param function the task function to execute
 * @return pointer to the created task
 */
Task* CreateChildTask(Task* parent, TaskFunction function);

/**
 * create a child task with data
 * @param parent pointer to the parent of this task
 * @param function the task function to execute
 * @param taskData pointer to input data for the task
 * @param taskDataSize the size of the memory block the pointer points to
 * @return pointer to the created task
 */
Task* CreateChildTask(Task* parent, TaskFunction function, const void* taskData,
                      size_t taskDataSize);

/**
 * Run a task, which adds it to the task queue,
 * enabling other workers to steal it
 * @param task pointer to task to run
 */
void Run(Task* task);

/**
 * Wait for a task to finish, and while waiting, help with executing tasks
 * @param task pointer to task to wait for
 */
void Wait(const Task* task);

/**
 * Task function which does nothing (useful for empty root tasks)
 * @param task null task
 * @param taskData null data
 */
void EmptyTask(Task* task, const void* taskData);

} // namespace Scheduler
} // namespace bge