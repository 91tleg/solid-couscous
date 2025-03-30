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
    OCS_PRESS,   // Long press detected
    OCS_PRESSEND // Button released after long press
} button_state_t;

void button_task(void *pvParameter);
void reset_button_state(void);
int debounce(int value);

extern void (*click_callback)(void);
extern void (*double_click_callback)(void);
extern void (*long_press_callback)(void);
extern void (*long_press_end_callback)(void);

#endif // BUTTON_H
