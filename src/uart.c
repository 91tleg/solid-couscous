#include "uart.h"
#include "driver/gpio.h"
#include "assert_handler.h"

#define UART_RX_BUF_SIZE (2048)
#define UART_TX_BUF_SIZE (1024)

static void uart_configure(void)
{
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(
        UART_NUM,
        TX_PIN,
        RX_PIN,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE);
    uart_driver_install(
        UART_NUM,
        UART_RX_BUF_SIZE,
        UART_TX_BUF_SIZE,
        0,
        NULL,
        0);
}

bool uart_initialized = false;
void uart_init(void)
{
    ASSERT(!uart_initialized);
    uart_configure();
    uart_initialized = true;
}