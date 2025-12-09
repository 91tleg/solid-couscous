#include "uart_task.h"
#include "fsm_states.h"
#include "ssm1.h"
#include "addr_map.h"
#include "log.h"
#include "defines.h"
#include "uart_data.h"
#include "uart.h"
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#define TAG             "UartTask"
#define TASK_STACK_SIZE (4096U)

static StaticTask_t uart_task_tcb;
static StackType_t uart_task_stack[TASK_STACK_SIZE];
static TaskHandle_t uart_task_handle = NULL;
static QueueHandle_t uart_data_queue = NULL;

static void uart_task(void *parameters)
{
    uint8_t rxbuf[128];
    uint8_t cmd[4];

    struct ssm1_parser parser = {0};
    struct romid_ctx romctx = {0};
    struct read_ctx  rctx  = {0};

    TickType_t last_romid_send = 0;
    bool romid_received = false;

    for (;;)
    {
        // Retry ROMID until received
        if (!romid_received &&
            (xTaskGetTickCount() - last_romid_send) > pdMS_TO_TICKS(100))
        {
            ssm1_get_romid_command(&romctx, cmd);
            send_bytes(cmd, 4);
            last_romid_send = xTaskGetTickCount();
        }

        int len = uart_read_from_isr(rxbuf, sizeof(rxbuf), 5);
        if (len > 0)
        {
            struct parsed_msg msgs[8];
            size_t count = ssm1_parser_feed(&parser, rxbuf, len, msgs, 8);

            for (size_t i = 0; i < count; i++)
            {
                struct parsed_msg *m = &msgs[i];

                if (m->type == MSG_TYPE_ROMID)
                {
                    romid_received = true;

                    struct uart_data out = {0};
                    out.state = STATE_ROMID;
                    memcpy(out.romid, m->u.rom.romid, 3);
                    xQueueSend(uart_data_queue, &out, 0);
                }
                else if (m->type == MSG_TYPE_READ)
                {
                    struct uart_data out = {0};
                    out.state = rctx.state;
                    out.value = m->u.read.value;
                    xQueueSend(uart_data_queue, &out, 0);

                    ssm1_get_read_command(&rctx, cmd);
                    send_bytes(cmd, 4);
                }
            }
        }

        uint32_t new_state;
        if (xTaskNotifyWait(0, 0xFFFFFFFF, &new_state, 0) == pdPASS)
        {
            rctx.state = (fsm_state_e)new_state;
            rctx.addr  = lookup_addr(rctx.state);

            ssm1_get_read_command(&rctx, cmd);
            send_bytes(cmd, 4);
        }
    }
}

TaskHandle_t uart_task_init(QueueHandle_t uart_data_q)
{
    uart_data_queue = uart_data_q;

    uart_task_handle = xTaskCreateStaticPinnedToCore(
        uart_task,
        TAG,
        TASK_STACK_SIZE,
        NULL,
        1,
        uart_task_stack,
        &uart_task_tcb,
        0
    );
    LOGI(TAG, "Task started");

    return uart_task_handle;
}
