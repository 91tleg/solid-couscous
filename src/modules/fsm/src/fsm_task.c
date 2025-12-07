#include "fsm_task.h"
#include "fsm.h"
#include "fsm_states.h"
#include "button_event.h"
#include "log.h"
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#define TAG             "FsmTask"
#define TASK_STACK_SIZE (2048U)

static StaticTask_t fsm_task_tcb;
static StackType_t fsm_task_stack[TASK_STACK_SIZE];
static QueueHandle_t button_event_queue;
static TaskHandle_t uart_task_handle;

static void fsm_task(void *parameters)
{
    fsm_state_e state = STATE_ROMID;
    button_event_e next_event;

    for (;;)
    {
        // Block until new event
        if (xQueueReceive(button_event_queue, &next_event, portMAX_DELAY))
        {
            fsm_process_event(&state, next_event);
            // Notify uart task of state change
            xTaskNotify(uart_task_handle,
                        (uint8_t)state,
                        eSetValueWithOverwrite);
        }
        vTaskDelay(pdTICKS_TO_MS(10));
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
        3,
        fsm_task_stack,
        &fsm_task_tcb
    );
    LOGI(TAG, "Task started");
}
