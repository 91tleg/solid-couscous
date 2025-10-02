#include "mocks/freertos/task.h"
#include "mocks/freertos/queue.h"
#include "mocks/freertos/FreeRTOS.h"
#include "mocks/button/button.h"
#include "mocks/state_machine/state_machine.h"

#include "modules/button/button_task.h"
#include "modules/state_machine/state_machine_task.h"
#include "state_defs.h"
#include <stdio.h>
#include <unistd.h>
#include <unity.h>

void test_button_state_machine(void)
{
    button_task_init();
    state_machine_task_init();

    QueueHandle_t button_q = button_get_event_queue();
    TEST_ASSERT_NOT_NULL(button_q);

    QueueHandle_t lcd_q = state_machine_get_lcd_queue();
    TEST_ASSERT_NOT_NULL(lcd_q);

    state_event_e event = STATE_EVENT_BUTTON_PRESS;
    int res = xQueueSend(button_q, &event, 1000);
    TEST_ASSERT_EQUAL_INT(pdPASS, res);

    vTaskDelay(500);

    struct state_machine_data data;
    res = xQueueReceive(lcd_q, &data, 1000);
    TEST_ASSERT_EQUAL_INT(pdPASS, res);

    printf("[C TEST] LCD state = %d\n", data.state);
    TEST_ASSERT_EQUAL(STATE_BATTERY_V, data.state);

    printf("[C TEST] Passed: button press propagated to state machine\n");
}