#include "uart.h"
#include "board.h"
#include "log.h"
#include <driver/uart.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define TAG              "Uart"
#define UART_BAUD_RATE   (1953)
#define UART_RX_BUF_SIZE (512)
#define UART_TX_BUF_SIZE (1024)

static QueueHandle_t uart_isr_queue = NULL;

void uart_driver_init(void)
{
    uart_config_t uart_conf = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(UART_NUM, &uart_conf);
    uart_set_pin(
        UART_NUM,
        TX_PIN,
        RX_PIN,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE
    );

    uart_driver_install(
        UART_NUM,
        UART_RX_BUF_SIZE,
        UART_TX_BUF_SIZE,
        20,
        &uart_isr_queue,
        0
    );

    LOGI(TAG, "UART initialized");
}

bool send_bytes(const uint8_t *data, uint8_t len)
{
    int written = uart_write_bytes(UART_NUM, data, len);
    return written == len;
}

int read_bytes(uint8_t *buf, uint32_t max_len, uint32_t timeout)
{
    uart_event_t event;

    if (!xQueueReceive(uart_isr_queue, &event, (TickType_t)timeout)
        || event.type != UART_DATA)
    {
        return 0;  // no data
    }

    uint32_t to_read = event.size;
    if (to_read > max_len) { to_read = max_len; }
    int len = uart_read_bytes(UART_NUM, buf, to_read, 0);

    return len;
}
