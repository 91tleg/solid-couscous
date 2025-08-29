#include "uart.h"
#include "driver/gpio.h"
#include "assert_handler.h"

#define UART_RX_BUF_SIZE    (2048)
#define UART_TX_BUF_SIZE    (1024)
#define UART_QUEUE_SIZE     (20)

static QueueHandle_t uart_queue;

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
        UART_QUEUE_SIZE, 
        &uart_queue, 
        0
    );
}

bool uart_initialized = false;
void uart_init(void)
{
    ASSERT(!uart_initialized);
    uart_configure();
    uart_initialized = true;
}

inline bool send_byte(uint8_t b)
{
    int written = uart_write_bytes(UART_NUM, (const char*)&b, 1);
    return (written == 1);
}

inline bool read_byte(uint8_t *b)
{
    uart_event_t event;
    if (xQueueReceive(uart_queue, &event, 0) == pdPASS)
    {
        if (event.type == UART_DATA)
        {
            int len = uart_read_bytes(UART_NUM, b, 1, 0);
            return (len == 1);
        }
    }
    return false;
}