#ifndef BUTTON_TASK_H
#define BUTTON_TASK_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

/**
 * @brief Get the FreeRTOS queue used to send button events.
 * 
 * @return QueueHandle_t Handle to the button event queue.
 */
QueueHandle_t button_get_event_queue(void);

/**
 * @brief Button task to read the button and push events to the queue.
 *
 * This task should be run in a FreeRTOS environment. It continuously polls the button
 * using the `read_state_event()` function and pushes any detected events to the
 * button event queue.
 *
 * @param parameters FreeRTOS task parameters (unused).
 */
void button_task(void *parameters);

#endif // BUTTON_TASK_H