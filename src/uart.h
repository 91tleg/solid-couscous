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
bool send_byte(uint8_t b);
bool read_byte(uint8_t *b);

#endif