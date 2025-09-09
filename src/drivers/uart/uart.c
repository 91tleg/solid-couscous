#include "uart.h"

#define UART_RX_BUF_SIZE (512)
#define UART_TX_BUF_SIZE (1024)

void uart_init(void)
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
}