#ifndef UART_H
#define UART_H
#include "driver/uart.h"

#ifdef SSM1
    #define BAUD_RATE   (1953)
    #define UART_NUM    (UART_NUM_2)
    #define TX_PIN      (GPIO_NUM_25)
    #define RX_PIN      (GPIO_NUM_26)
#else
    #define BAUD_RATE   (1953)
    #define UART_NUM    (UART_NUM_0)
    #define TX_PIN      (GPIO_NUM_1)
    #define RX_PIN      (GPIO_NUM_3)
#endif

void uart_init(void);

static inline bool send_bytes(const uint8_t *data, uint8_t len)
{
    int written = uart_write_bytes(UART_NUM, data, len);
    return (written == (int)len);
}

static inline int read_bytes(uint8_t *buf, uint8_t max_len)
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

    return uart_read_bytes(UART_NUM, buf, available, 0);
}

#endif