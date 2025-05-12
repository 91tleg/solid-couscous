#include "uart.h"
#include "driver/gpio.h"
#include "assert_handler.h"

#ifdef SSM1
    #define BAUD_RATE   (1953)
    #define TX_PIN      (GPIO_NUM_17)
    #define RX_PIN      (GPIO_NUM_16)
#else
    #define BAUD_RATE   (9600)
    #define TX_PIN      (GPIO_NUM_1)
    #define RX_PIN      (GPIO_NUM_3)
#endif

static void uart_configure(void)
{
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, 2048, 0, 0, NULL, 0);
}

bool initialized = false;
void uart_init(void)
{
    ASSERT(!initialized);
    uart_configure();
    initialized = true;
}