#ifndef BUTTON_H
#define BUTTON_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern QueueHandle_t event_queue;

typedef enum
{
    STATE_EVENT_BUTTON_PRESS,
    STATE_EVENT_BUTTON_LONG_PRESS,
    STATE_EVENT_BUTTON_DOUBLE_PRESS,
    STATE_EVENT_NONE
} state_event_e;

void button_task(void *parameters);

#endif // BUTTON_H
