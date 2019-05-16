#pragma once

#include "ParallelForSplitters.h"
#include "Scheduler.h"

namespace bge
{

/**
 * Data that the parralel for task uses
 */
template <typename T, typename S> struct ParralelForTaskData
{
  using DataType = T;
  using SplitterType = S;

  ParralelForTaskData(DataType* data, uint32 count,
                      void (*function)(DataType*, uint32),
                      const SplitterType& splitter)
      : m_Function(function)
      , m_Data(data)
      , m_Count(count)
      , m_Splitter(splitter)
  {
  }

  void (*m_Function)(DataType*, uint32);
  DataType* m_Data;
  uint32 m_Count;
  SplitterType m_Splitter;
};

/**
 * The parralel for task function, which splits the passed data until it
 * doesn't meet the split requirements and  then executes all the leaves
 * example of splitting 5 elements into loops of 2 (or lower)
 *       *  - 5 elements
 *      / \
 *     *   * - 3/2 elements (execute 2)
 *    / \
 *   *   *    - 2/1 (execute 2 and 1)
 */
template <typename TaskData>
void ParallelForTask(Task* task, const void* taskData)
{
  const TaskData* data = static_cast<const TaskData*>(taskData);
  const TaskData::SplitterType& splitter = data->m_Splitter;

  if (splitter.Split<TaskData::DataType>(data->m_Count))
  {
    // Left side
    const uint32 leftCount = data->m_Count / 2u;
    const TaskData leftData(data->m_Data, leftCount, data->m_Function,
                            splitter);

    Task* left = Scheduler::CreateChildTask(task, ParallelForTask<TaskData>,
                                            &leftData, sizeof(leftData));
    Scheduler::Run(left);

    // Right side
    const uint32 rightCount = data->m_Count - leftCount;
    const TaskData rightData(data->m_Data + leftCount, rightCount,
                             data->m_Function, splitter);
    Task* right = Scheduler::CreateChildTask(task, ParallelForTask<TaskData>,
                                             &rightData, sizeof(rightData));
    Scheduler::Run(right);
  }
  else
  {
    (data->m_Function)(data->m_Data, data->m_Count);
  }
}

/**
 * Executes a task which splits the passed data based on the splitter and once
 * it reaches the maximum amount of splits it can, it executes the leaves
 * @param data pointer to array of data for the task
 * @param count number of elements in array
 * @param function task to execute on every element of the data
 * @param splitter how to split the data into multiple tasks
 */
template <typename DataType, typename SplitterType>
Task* ParralelFor(DataType* data, uint32 count,
                  void (*function)(DataType*, uint32),
                  const SplitterType& splitter)
{
  using TaskData = ParralelForTaskData<DataType, SplitterType>;

  const TaskData taskData(data, count, function, splitter);

  Task* task = Scheduler::CreateTask(ParallelForTask<TaskData>, &taskData,
                                     sizeof(taskData));
  return task;
}

} // namespace bge