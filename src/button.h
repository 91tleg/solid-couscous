#ifndef BUTTON_H
#define BUTTON_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define BTN_PIN             (34)
#define DEBOUNCE_TIME       (50000UL)
#define SHORT_PRESS_TIME    (400000UL)
#define DOUBLE_CLICK_TIME   (400000UL)
#define LONG_PRESS_TIME     (1000000UL)

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
