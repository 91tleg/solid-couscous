#ifndef UART_H
#define UART_H
#include "driver/uart.h"

#ifdef SSM1
    #define UART_NUM (UART_NUM_1)
#else
    #define UART_NUM (UART_NUM_0)
#endif

void uart_init(void);

#endif