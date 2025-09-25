#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include "mocks/button/button_task.h"
#include "mocks/state_machine/state_machine_task.h"

#include "modules/button/button_task.h"
#include "modules/state_machine/state_machine_task.h"
#include "state_defs.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

void test_button_state_machine(void)
{
    xTaskCreate(button_task, "ButtonTask", 1024, NULL, 1, NULL);
    xTaskCreate(state_machine_task, "StateTask", 1024, NULL, 1, NULL);

    QueueHandle_t button_q = button_get_event_queue();
    QueueHandle_t lcd_q = state_machine_get_lcd_queue();

    state_event_e event = STATE_EVENT_BUTTON_PRESS;
    int res = xQueueSend(button_q, &event, 1000);
    assert(res == pdPASS);

    vTaskDelay(500);

    struct state_machine_data data;
    res = xQueueReceive(lcd_q, &data, 1000);
    assert(res == pdPASS);

    printf("[C TEST] LCD state = %d\n", data.state);
    assert(data.state == STATE_BATTERY_V);

    printf("[C TEST] Passed: button press propagated to state machine\n");
}