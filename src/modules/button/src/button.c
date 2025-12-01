#include "button.h"
#include "timer.h"
#include "log.h"

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

static __attribute__((unused)) int button_debounce(int raw_state)
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

static inline void button_state_reset(void)
{
    state = BUTTON_STATE_INIT;
    click_count = 0;
    press_start_time = 0;
}

button_event_e button_process_event(uint8_t level, uint64_t timestamp)
{
    uint64_t press_duration = timestamp - press_start_time;
    button_event_e event = BUTTON_EVENT_NONE;

    switch (state)
    {
    case BUTTON_STATE_INIT:
        if (level == BUTTON_PRESSED)
        {
            state = BUTTON_STATE_PRESSED;
            press_start_time = timestamp;
            click_count = 0;
            event = BUTTON_EVENT_NONE;
        }
        break;
    case BUTTON_STATE_PRESSED:
        if (timestamp - press_start_time > LONG_PRESS_TIME_US)
        {
            event = BUTTON_EVENT_LONG_PRESS;
            state = BUTTON_STATE_HELD;
            LOGI(TAG, "Long press");
        }
        if(level == BUTTON_RELEASED)
        {
            state = BUTTON_STATE_RELEASED;
            press_start_time = timestamp;
        }
        break;
    case BUTTON_STATE_RELEASED:
        click_count++;
        state = BUTTON_STATE_COUNTING;
        break;
    case BUTTON_STATE_COUNTING:
        if (level == BUTTON_PRESSED)
        {
            state = BUTTON_STATE_PRESSED;
            press_start_time = timestamp;
        }
        else if ((press_duration >= SHORT_PRESS_TIME_US)
                  || (click_count == 2))
        {
            if (click_count == 1)
            {
                event = BUTTON_EVENT_SINGLE_PRESS;
                LOGI(TAG, "Single Press");
            }
            else if (click_count == 2)
            {
                event = BUTTON_EVENT_DOUBLE_PRESS;
                LOGI(TAG, "Double Press");
            }
            button_state_reset();
        }
        break;
    case BUTTON_STATE_HELD:
        if (level == BUTTON_RELEASED)
        {
            state = BUTTON_STATE_HELD_RELEASED;
            press_start_time = timestamp;
        }
        break;
    case BUTTON_STATE_HELD_RELEASED:
        button_state_reset();
        break;
    }
    return event;
}