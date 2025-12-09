#ifndef UART_DATA_H
#define UART_DATA_H

#include "fsm_states.h"
#include <stdint.h>

struct uart_data
{
    fsm_state_e state;
    uint8_t value;
    uint8_t romid[3];
};

#endif // UART_DATA