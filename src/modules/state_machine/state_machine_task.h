#ifndef STATE_MACHINE_TASK_H
#define STATE_MACHINE_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

/**
 * @brief Retrieves the lcd queue handle used for communication between tasks.
 *
 * @return QueueHandle_t Handle to the lcd queue.
 */
QueueHandle_t state_machine_get_lcd_queue(void);

/**
 * @brief State machine task for managing system states.
 *
 * This function should be run as a FreeRTOS task. It handles
 * transitions and actions based on system events.
 *
 * @param parameters Pointer to task-specific parameters (unused).
 */
void state_machine_task(void *parameters);

#endif // STATE_MACHINE_TASK_H