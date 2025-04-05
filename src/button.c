#include "button.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "state_machine.h"

QueueHandle_t event_queue;

typedef enum
{
    OCS_INIT,    // Initial state
    OCS_DOWN,    // Button pressed
    OCS_UP,      // Button released
    OCS_COUNT,   // Count the number of clicks
    OCS_PRESS,   // button is hold down
    OCS_PRESSEND // Button released after long press
} button_state_e;

static button_state_e state = OCS_INIT;
static unsigned long last_debounce_time = 0;
static unsigned long press_start_time = 0;
static unsigned int click_count = 0;

static int debounce(int value)
{
    static int last_state = 1;
    static int debounced_state = 1;
    unsigned long current_time = esp_timer_get_time();
    if (value != last_state)
    {
        last_debounce_time = current_time;
    }
    if ((current_time - last_debounce_time) > DEBOUNCE_TIME)
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

static void button_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BTN_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};
    
    gpio_config(&io_conf);
}

static state_event_e read_state_event(void)
{
    int button_state = gpio_get_level(BTN_PIN);
    int debounced_state = debounce(button_state);
    unsigned long current_time = esp_timer_get_time();
    unsigned long press_duration = current_time - press_start_time;
    state_event_e event = STATE_EVENT_NONE;

    switch (state)
    {
    case OCS_INIT:
        if (debounced_state == 0)  // Pressed
        {
            state = OCS_DOWN;
            press_start_time = current_time;
        }
        break;

    case OCS_DOWN:
        if (debounced_state == 1)
        {
            if (press_duration >= LONG_PRESS_TIME)
            {
                state = OCS_PRESSEND;
                event = STATE_EVENT_BUTTON_LONG_PRESS;
                ESP_LOGI("BTN", "Long Press");
            }
            else
            {
                state = OCS_UP;
                press_start_time = current_time;
            }
        }
        break;

    case OCS_UP:
        click_count++;
        state = OCS_COUNT;
        break;

    case OCS_COUNT:
        if (debounced_state == 0)
        {
            state = OCS_DOWN;
            press_start_time = current_time;
        }
        else if ((current_time - press_start_time) >= DOUBLE_CLICK_TIME)
        {
            // Times up, determine single or double press
            if (click_count == 1)
            {
                event = STATE_EVENT_BUTTON_PRESS;
                ESP_LOGI("BTN", "Single Press");
            }
            else if (click_count == 2)
            {
                event = STATE_EVENT_BUTTON_DOUBLE_PRESS;
                ESP_LOGI("BTN", "Double Press");
            }
            reset_button_state();
        }
        break;

    case OCS_PRESS:
        if (debounced_state == 1)
        {
            state = OCS_PRESSEND;
        }
        break;

    case OCS_PRESSEND:
        reset_button_state();
        break;
    }

    return event;
}

void button_task(void *parameters)
{
    button_init();
    reset_button_state();
    state_event_e event = STATE_EVENT_NONE;

    for (;;)
    {
        event = read_state_event();
        if (event != STATE_EVENT_NONE)
        {
            xQueueSend(event_queue, (void *)&event, portMAX_DELAY); 
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

