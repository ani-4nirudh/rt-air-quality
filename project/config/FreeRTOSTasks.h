#ifndef FREERTOSTASKS_H_
#define FREERTOSTASKS_H_

#include "FreeRTOS.h"
#include "task.h"

/**
 * User defined config variables to define the startup task.
 * This task is supposd to have the highest priority compared to the other tasks that will be created.
 * Although this will be deleted later.
 */
#define STARTUP_TASK_STACK_SIZE (512)

// Highest priority that can be assigned to a task by FreeRTOS
#define STARTUP_TASK_PRIORITY (configMAX_PRIORITIES - 1)

#endif // !FREERTOSTASKS_H_
