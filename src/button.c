#include "button.h"

static button_state_t state = OCS_INIT;
static unsigned long last_debounce_time = 0;
static unsigned long press_start_time = 0;
static unsigned int click_count = 0;

void (*click_callback)(void) = NULL;
void (*double_click_callback)(void) = NULL;
void (*long_press_callback)(void) = NULL;
void (*long_press_end_callback)(void) = NULL;

int debounce(int value)
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

void reset_button_state(void)
{
    state = OCS_INIT;
    click_count = 0;
    press_start_time = 0;
}

void button_task(void *pvParameter)
{
    while (1)
    {
        int button_state = gpio_get_level(BTN_PIN);
        int debounced_state = debounce(button_state);
        unsigned long wait_time = esp_timer_get_time() - press_start_time;

        switch (state)
        {
        case OCS_INIT:
            if (debounced_state == 0)
            {
                state = OCS_DOWN;
                press_start_time = esp_timer_get_time();
            }
            break;

        case OCS_DOWN:
            if (debounced_state == 1)
            {
                state = OCS_UP;
                unsigned long press_duration = esp_timer_get_time() - press_start_time;

                if (press_duration >= LONG_PRESS_TIME)
                {
                    if (long_press_callback)
                    {
                        long_press_callback();
                    }
                    state = OCS_PRESSEND;
                }
                else
                {
                    state = OCS_COUNT;
                }
            }
            break;

        case OCS_UP:
            if (debounced_state == 0)
            {
                state = OCS_DOWN;
                press_start_time = esp_timer_get_time();
            }
            else
            {
                if (wait_time >= SHORT_PRESS_TIME)
                {
                    if (click_count == 1 && click_callback)
                    {
                        click_callback();
                    }
                    else if (click_count == 2 && double_click_callback)
                    {
                        double_click_callback();
                    }
                    reset_button_state();
                }
            }
            break;

        case OCS_COUNT:
            if (debounced_state == 0)
            {
                state = OCS_DOWN;
                press_start_time = esp_timer_get_time();
            }
            else
            {
                if (wait_time >= DOUBLE_CLICK_TIME)
                {
                    if (click_count == 1 && click_callback)
                    {
                        click_callback();
                    }
                    else if (click_count == 2 && double_click_callback)
                    {
                        double_click_callback();
                    }
                    reset_button_state();
                }
            }
            break;

        case OCS_PRESSEND:
            if (debounced_state == 1)
            {
                if (long_press_end_callback)
                {
                    long_press_end_callback();
                }
                reset_button_state();
            }
            break;

        default:
            reset_button_state();
            break;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
