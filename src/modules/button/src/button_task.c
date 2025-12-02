#include "button_task.h"
#include "button_driver.h"
#include "button.h"
#include "button_event.h"
#include "assert_handler.h"
#include "log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#define TAG             "ButtonTask"
#define TASK_STACK_SIZE (2048U)

static StaticTask_t button_task_tcb;
static StackType_t button_task_stack[TASK_STACK_SIZE];
static TaskHandle_t button_task_handle = NULL;

static Queuehandle_t button_isr_queue = NULL;
static QueueHandle_t button_event_queue = NULL;


static void button_task(void *parameters)
{
    struct button_isr raw;
    button_event_e evt;

    for (;;)
    {
        // Will block until the ISR pushes an event
        xQueueReceive(button_isr_queue, &raw, portMAX_DELAY);

        evt = button_process_event(raw.level, raw.timestamp_us);
        if (evt != BUTTON_EVENT_NONE)
        {
            if (xQueueSend(button_event_queue, &evt, 0) != pdPASS)
            {
                LOGW(TAG, "Event queue full, dropping event");
            }
        }
    }
}

void button_task_init(QueueHandle_t isr_q, 
                      QueueHandle_t event_q)
{
    button_isr_queue = isr_q;
    button_event_queue = event_q;

    button_task_handle = xTaskCreateStatic(
        button_task,
        TAG,
        TASK_STACK_SIZE,
        NULL,
        3,
        button_task_stack,
        &button_task_tcb
    );
    LOGI(TAG, "Task started");
}
