#include "uart.h"
#include <driver/uart.h>
#include "board.h"
#include "core/log/log.h"
#include <stdio.h>

#define TAG              "Uart"
#define BAUD_RATE        (1953)
#define UART_RX_BUF_SIZE (512)
#define UART_TX_BUF_SIZE (1024)

void uart_driver_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM, &uart_config);
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
        0,
        NULL,
        0
    );

    LOGI(TAG, "Uart initialized");
}

bool send_bytes(const uint8_t *data, uint8_t len)
{
    int written = uart_write_bytes(UART_NUM, data, len);
    
    char log_buf[3 * len + 1]; // 2 hex chars + space + null
    for (int i = 0; i < len; i++) 
    {
        snprintf(&log_buf[i * 3], sizeof(log_buf) - (i * 3), "%02X ", log_buf[i]);
    }
    log_buf[3 * len] = '\0'; // null terminate
    LOGI(TAG, "Sent data: %s", log_buf);

    return (written == (int)len);
}

int read_bytes(uint8_t *buf, uint8_t max_len)
{
    size_t available = 0;
    uart_get_buffered_data_len(UART_NUM, &available);

    if (available == 0)
    {
        return 0;
    }

    if (available > max_len)
    {
        available = max_len;
    }

    int read = uart_read_bytes(UART_NUM, buf, (uint32_t)available, 0);

    char log_buf[3 * read + 1];
    for (int i = 0; i < read; i++)
    {
        snprintf(&log_buf[i * 3], sizeof(log_buf) - (i * 3), "%02X ", log_buf[i]);
    }
    log_buf[3 * read] = '\0';
    LOGI(TAG, "Received data: %s", log_buf);

    return read;
}