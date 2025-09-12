#include "button_task.h"
#include "button.h"
#include "drivers/button/button.h"
#include "core/assert/assert_handler.h"
#include "core/log/log.h"
#include <freertos/task.h>

#define TAG "ButtonTask"

static QueueHandle_t event_queue = NULL;

static void button_queue_init(void)
{
   if (event_queue == NULL)
    {
        event_queue = xQueueCreate(10, sizeof(state_event_e));
        LOGI(TAG, "Event queue initialized");
        ASSERT(event_queue != NULL);
    }
}

QueueHandle_t button_get_event_queue(void) 
{
    return event_queue;
}

void button_task(void *parameters)
{
    button_driver_init();
    button_queue_init();
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