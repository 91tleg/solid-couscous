#include "display_task.h"
#include "display.h"
#include "log.h"
#include "assert_handler.h"
#include "fsm_data.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#define TAG             "DisplayTask"
#define TASK_STACK_SIZE (4096U)

static StaticTask_t display_task_tcb;
static StackType_t display_task_stack[TASK_STACK_SIZE];
static QueueHandle_t display_queue = NULL;

static void display_task(void *parameters)
{
    struct fsm_data sm_data;

    for (;;)
    {
        xQueueReceive(display_queue, &sm_data, portMAX_DELAY);
        display_print_state(&sm_data);
        vTaskDelay(pdMS_TO_TICKS(40));
    }
}

void display_task_init(QueueHandle_t display_q)
{
    display_queue = display_q;

    xTaskCreateStatic(
        display_task,
        TAG,
        TASK_STACK_SIZE,
        NULL,
        2,
        display_task_stack,
        &display_task_tcb
    );
    LOGI(TAG, "Task started");
}
