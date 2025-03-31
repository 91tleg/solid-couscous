#include "button.h"
#include "uart.h"

static button_state_e state = OCS_INIT;
static unsigned long last_debounce_time = 0;
static unsigned long press_start_time = 0;
static unsigned int click_count = 0;

static int debounce(int value)
{
    static int last_state = 1;
    static int debounced_state = 1;

    if (value != last_state)
    {
        last_debounce_time = esp_timer_get_time();
    }

    if ((esp_timer_get_time() - last_debounce_time) > DEBOUNCE_TIME)
    {
        debounced_state = value;
    }

    last_state = value;
    return debounced_state;
}

static inline void reset_button_state(void)
{
    state = OCS_INIT;
    click_count = 0;
    press_start_time = 0;
}

state_event_e read_state_event(void)
{
    int button_state = gpio_get_level(BTN_PIN);
    int debounced_state = debounce(button_state);
    unsigned long current_time = esp_timer_get_time();
    unsigned long press_duration = current_time - press_start_time;
    state_event_e event = STATE_EVENT_NONE;

    switch (state)
    {
    case OCS_INIT:
        if (debounced_state == 0) // Button pressed
        {
            state = OCS_DOWN;
            press_start_time = current_time;
        }
        break;

    case OCS_DOWN:
        if (debounced_state == 1) // Button released
        {
            if (press_duration >= LONG_PRESS_TIME)
            {
                state = OCS_PRESSEND;
                event = STATE_EVENT_BUTTON_LONG_PRESS;
            }
            else
            {
                click_count++;
                state = OCS_COUNT; // Wait to check for double-click
            }
        }
        break;

    case OCS_UP:
        if (debounced_state == 0) // Button pressed again
        {
            state = OCS_DOWN;
            press_start_time = current_time;
        }
        else if (press_duration >= SHORT_PRESS_TIME)
        {
            if (click_count == 1)
            {
                event = STATE_EVENT_BUTTON_PRESS;
            }
            else if (click_count == 2)
            {
                event = STATE_EVENT_BUTTON_DOUBLE_PRESS;
            }
            reset_button_state();
        }
        break;

    case OCS_COUNT:
        if (debounced_state == 0)
        {
            state = OCS_DOWN;
            press_start_time = current_time;
        }
        else if (press_duration >= DOUBLE_CLICK_TIME)
        {
            if (click_count == 1)
            {
                event = STATE_EVENT_BUTTON_PRESS;
            }
            else if (click_count == 2)
            {
                event = STATE_EVENT_BUTTON_DOUBLE_PRESS;
            }
            reset_button_state();
        }
        break;

    case OCS_PRESS:
        if (debounced_state == 1) // Button released
        {
            state = OCS_PRESSEND;
        }
        break;

    case OCS_PRESSEND:
        if (debounced_state == 1) // Button fully released
        {
            reset_button_state();
        }
        break;
    }

    return event;
}
