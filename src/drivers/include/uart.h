#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stdint.h>

void uart_driver_init(void);
bool send_bytes(const uint8_t *data, uint8_t len);
int uart_read_from_isr(uint8_t *buf, uint32_t max_len, uint32_t timeout);

#endif // UART_H