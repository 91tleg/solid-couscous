#include "uart_task.h"
#include "fsm_states.h"
#include "ssm1.h"
#include "addr_map.h"
#include "log.h"
#include "uart_data.h"
#include "uart.h"
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
    uint8_t rxbuf[256] = {0};
    uint8_t cmd[4] = {0};

    struct ssm1_parser parser = {0};
    struct romid_ctx romctx = {0};
    struct read_ctx  rctx  = {0};

    struct uart_data out = {0};
    struct parsed_msg msgs[32] = {0};
 
    TickType_t last_romid_send = 0;
    bool romid_received = false;
    bool init_romid = false;
    uint32_t new_state = 0;

    for (;;)
    {
        if (!init_romid)
        {
            xQueueSend(uart_data_queue, &out, 0);
            init_romid = true;
            ssm1_get_romid_command(&romctx, cmd);
        }

        TickType_t now = xTaskGetTickCount();
        if (!romid_received &&
            (now - last_romid_send) > pdMS_TO_TICKS(1000))
        {
            send_bytes(cmd, 4);
            last_romid_send = now;
        }

        int len = uart_read_from_isr(rxbuf, sizeof(rxbuf), 1);
        if (len > 0)
        {
            size_t count = ssm1_parser_feed(&parser, rxbuf, len, msgs, 32);

            // Check first message for ROM ID
            if (msgs[0].type == MSG_TYPE_ROMID)
            {
                romid_received = true;
                rctx.addr = 0;
                ssm1_get_stop_command(&rctx, cmd);
                send_bytes(cmd, 4);

                out.state = STATE_ROMID;
                out.romid[0] = msgs[0].u.rom.romid[0];
                out.romid[1] = msgs[0].u.rom.romid[1];
                out.romid[2] = msgs[0].u.rom.romid[2];
                xQueueSend(uart_data_queue, &out, 0);
            }

            // Process remaining messages (skip if ROM ID)
            for (size_t i = 0; i < count; ++i)
            {
                struct parsed_msg *m = &msgs[i];
                if (m->type == MSG_TYPE_ROMID) { continue; }
                out.state = rctx.state;
                out.value = m->u.read.value;
                xQueueSend(uart_data_queue, &out, 0);
            }
        }

        if (xTaskNotifyWait(0, 0xFFFFFFFF, &new_state, 0) == pdPASS)
        {
            ssm1_get_stop_command(&rctx, cmd);
            send_bytes(cmd, 4);

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
