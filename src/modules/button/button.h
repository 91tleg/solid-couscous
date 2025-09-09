#ifndef BUTTON_H
#define BUTTON_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

typedef enum
{
    STATE_EVENT_BUTTON_PRESS,
    STATE_EVENT_BUTTON_LONG_PRESS,
    STATE_EVENT_BUTTON_DOUBLE_PRESS,
    STATE_EVENT_NONE
} state_event_e;

QueueHandle_t button_get_event_queue(void);
void button_task(void *parameters);

#endif // BUTTON_H