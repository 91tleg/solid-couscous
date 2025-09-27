#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stdint.h>

void uart_driver_init(void);
bool send_bytes(const uint8_t *data, uint8_t len);
int read_bytes(uint8_t *buf, uint8_t max_len);

#endif // UART_H