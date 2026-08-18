#include "fsm_task.h"
#include "fsm.h"
#include "fsm_states.h"
#include "button_event.h"
#include "log.h"
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#define TAG "FsmTask"
#define TASK_STACK_SIZE (4096U)

static StaticTask_t fsm_task_tcb;
static StackType_t fsm_task_stack[TASK_STACK_SIZE];
static QueueHandle_t button_event_queue = NULL;
static TaskHandle_t uart_task_handle = NULL;

static void fsm_task(void *parameters)
{
    fsm_state_e state = STATE_ROMID;
    button_event_e next_event = BUTTON_EVENT_NONE;

    for (;;)
    {
        xQueueReceive(button_event_queue, &next_event, portMAX_DELAY);

        fsm_process_event(&state, next_event);

        xTaskNotify(uart_task_handle,
                    (uint32_t)state,
                    eSetValueWithOverwrite);
    }
}

void fsm_task_init(QueueHandle_t btn_evt_q, TaskHandle_t uart_task_h)
{
    button_event_queue = btn_evt_q;
    uart_task_handle = uart_task_h;

    xTaskCreateStatic(
        fsm_task,
        TAG,
        TASK_STACK_SIZE,
        NULL,
        5,
        fsm_task_stack,
        &fsm_task_tcb
    );
    LOGI(TAG, "Task started");
}
