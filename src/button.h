#ifndef BUTTON_H
#define BUTTON_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "driver/gpio.h"

#define BTN_PIN (34)
#define DEBOUNCE_TIME (50)
#define SHORT_PRESS_TIME (500)
#define DOUBLE_CLICK_TIME (500)
#define LONG_PRESS_TIME (1500)

typedef enum
{
    OCS_INIT,    // Initial state
    OCS_DOWN,    // Button pressed
    OCS_UP,      // Button released
    OCS_COUNT,   // Count the number of clicks
    OCS_PRESS,   // button is hold down
    OCS_PRESSEND // Button released after long press
} button_state_e;

typedef enum
{
    STATE_EVENT_BUTTON_PRESS,
    STATE_EVENT_BUTTON_LONG_PRESS,
    STATE_EVENT_BUTTON_DOUBLE_PRESS,
    STATE_EVENT_NONE
} state_event_e;

state_event_e read_state_event(void);

#endif // BUTTON_H
