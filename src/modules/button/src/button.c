#include "button.h"
#include "drivers/button/button.h"
#include "core/timer/timer.h"
#include "core/log/log.h"

#define TAG                 "Button"
#define DEBOUNCE_TIME_US    (1000UL)
#define SHORT_PRESS_TIME_US (300000UL)
#define LONG_PRESS_TIME_US  (800000UL)
#define BUTTON_PRESSED      (0)
#define BUTTON_RELEASED     (1)

typedef enum
{
    BUTTON_STATE_INIT,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_RELEASED,
    BUTTON_STATE_COUNTING,
    BUTTON_STATE_HELD,
    BUTTON_STATE_HELD_RELEASED,
} button_state_e;

static unsigned long press_start_time = 0;
static unsigned int click_count = 0;
static button_state_e state = BUTTON_STATE_INIT;

static int debounce(int raw_state)
{
    static int debounced_state = BUTTON_RELEASED;
    static int last_state = BUTTON_RELEASED;
    static int64_t last_change_time_us = 0;
    int64_t now = timer_get_time_us();

    if (raw_state != last_state)
    {
        last_change_time_us = now;
        last_state = raw_state;
    }
    if ((now - last_change_time_us) >= DEBOUNCE_TIME_US)
    {
        debounced_state = last_state;
    }
    return debounced_state;
}

static inline void reset_button_state(void)
{
    state = BUTTON_STATE_INIT;
    click_count = 0;
    press_start_time = 0;
}

state_event_e read_state_event(void)
{
    int button_state = button_driver_read();
    int debounced_state = debounce(button_state);
    unsigned long current_time = (unsigned long)(timer_get_time_us());
    unsigned long press_duration = (current_time - press_start_time);
    state_event_e event = STATE_EVENT_NONE;

    switch (state)
    {
    case BUTTON_STATE_INIT:
        LOGI(TAG, "Init");
        if (debounced_state == BUTTON_PRESSED)
        {
            state = BUTTON_STATE_PRESSED;
            press_start_time = current_time;
            click_count = 0;
            event = STATE_EVENT_NONE;
        }
        break;
    case BUTTON_STATE_PRESSED:
        LOGI(TAG, "Pressed");
        if (debounced_state == 1)
        {
            state = BUTTON_STATE_RELEASED;
            press_start_time = current_time;
        }
        else if ((debounced_state == BUTTON_PRESSED) && (press_duration > LONG_PRESS_TIME_US))
        {
            event = STATE_EVENT_BUTTON_LONG_PRESS;
            LOGI(TAG, "Long Press");
            state = BUTTON_STATE_HELD;
        }
        break;
    case BUTTON_STATE_RELEASED:
        LOGI(TAG, "Released");
        click_count++;
        state = BUTTON_STATE_COUNTING;
        break;
    case BUTTON_STATE_COUNTING:
        if (debounced_state == BUTTON_PRESSED)
        {
            state = BUTTON_STATE_PRESSED;
            press_start_time = current_time;
        }
        else if ((press_duration >= SHORT_PRESS_TIME_US) || (click_count == 2))
        {
            if (click_count == 1)
            {
                event = STATE_EVENT_BUTTON_PRESS;
                LOGI(TAG, "Single Press");
            }
            else if (click_count == 2)
            {
                event = STATE_EVENT_BUTTON_DOUBLE_PRESS;
                LOGI(TAG, "Double Press");
            }
            reset_button_state();
        }
        break;
    case BUTTON_STATE_HELD:
        LOGI(TAG, "Held");
        if (debounced_state == BUTTON_RELEASED)
        {
            state = BUTTON_STATE_HELD_RELEASED;
            press_start_time = current_time;
        }
        break;
    case BUTTON_STATE_HELD_RELEASED:
        LOGI(TAG, "Held released");
        reset_button_state();
        break;
    }
    return event;
}