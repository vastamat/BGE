#include "scheduler/Task.h"

namespace bge
{

constexpr int sizeOfTask = sizeof(Task);

static_assert(sizeOfTask == 64, "Task must be 64 bytes to avoid false sharing");

} // namespace bge