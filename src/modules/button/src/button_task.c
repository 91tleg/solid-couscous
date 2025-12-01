#include "button_task.h"
#include "button.h"
#include "core/assert/assert_handler.h"
#include "core/log/log.h"
#include <freertos/task.h>

#define TAG "ButtonTask"

static QueueHandle_t event_queue = NULL;

void button_task_init(void)
{
    event_queue = xQueueCreate(10, sizeof(state_event_e));
    ASSERT(event_queue != NULL);
    LOGI(TAG, "Event queue initialized");

    BaseType_t res = xTaskCreate(
        button_task,
        "ButtonTask",
        2048,
        NULL,
        3,
        NULL
    );
    ASSERT(res == pdPASS);
    LOGI(TAG, "Button task started");
}

QueueHandle_t button_get_event_queue(void) 
{
    return event_queue;
}

void button_task(void *parameters)
{
    state_event_e event;

    for (;;)
    {
        event = read_state_event();
        if (event != STATE_EVENT_NONE)
        {
            xQueueSend(event_queue, &event, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}