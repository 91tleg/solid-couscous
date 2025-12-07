#include "state_machine_task.h"
#include "state_machine.h"
#include "modules/button/button_task.h"
#include "core/assert/assert_handler.h"
#include "core/log/log.h"
#include "state_defs.h"
#include <freertos/task.h>

#define TAG "StateMachineTask"

static QueueHandle_t lcd_queue = NULL;

static inline state_event_e process_input(void)
{
    state_event_e received_event;
    QueueHandle_t event_queue = button_get_event_queue();
    if (xQueueReceive(event_queue, &received_event, pdMS_TO_TICKS(0)) == pdPASS)
    {
        return received_event;
    }
    return STATE_EVENT_NONE;
}

void state_machine_task_init(void)
{
    lcd_queue = xQueueCreate(26, sizeof(struct state_machine_data));
    ASSERT(lcd_queue != NULL);
    LOGI(TAG, "Lcd queue initialized");

    BaseType_t res = xTaskCreate(
        state_machine_task,
        "State Machine Task",
        4096,
        NULL,
        2,
        NULL
    );
    ASSERT(res == pdPASS);
    LOGI(TAG, "State machine task started");
}

QueueHandle_t state_machine_get_lcd_queue(void)
{
    return lcd_queue;
}

void state_machine_task(void *parameters)
{
    static struct state_machine_data data;
    state_machine_data_init(&data);

    for (;;)
    {
        state_event_e next_event = process_input();
        process_event(&data, next_event);
        if (next_event != STATE_EVENT_NONE)
        {
            xQueueSend(lcd_queue, &data, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}