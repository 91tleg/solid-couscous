#include "uart.h"
#include <stdint.h>

void uart_init(void) 
{
    *(volatile uint32_t*)UART_CLKDIV_REG(UART1_BASE) = UART_CLK_DIV;

    // Config UART1
    *(volatile uint32_t*)UART_CONF0_REG(UART1_BASE) = (1 << 1) |  // 1 stop bit
                                                      (1 << 3) |  // Even parity
                                                      (1 << 4);   // 8 data bits
    // Enable TX FIFO
    *(volatile uint32_t*)UART_STATUS_REG(UART1_BASE) |= (1 << 2);
}

void uart_send_byte(uint8_t byte) 
{
    // Wait for space in the TX FIFO
    while (*(volatile uint32_t*)UART_STATUS_REG((uint32_t)UART1) & UART_TXFIFO_FULL);
    
    // Write byte to UART FIFO register
    *(volatile uint32_t*)UART_FIFO_REG((uint32_t)UART1) = byte;
}

uint8_t uart_read_byte(void) 
{
    // Wait until data is available in FIFO
    while (!(*(volatile uint32_t*)UART_INT_RAW_REG((uint32_t)UART1) & (1 << 0)));

    // Read from FIFO register
    return (uint8_t)(*(volatile uint32_t*)UART_FIFO_REG((uint32_t)UART1));
}

int uart_read_bytes(uint8_t* buf, int length)
{
    int count = 0;
    while (count < length) 
    {
        if (uart_available() > 0) 
        {
            buf[count++] = uart_read_byte();
        }
    }
    return count;
}


void uart_flush(void) 
{
    // Wait until TX FIFO is empty
    while (*(volatile uint32_t*)UART_STATUS_REG((uint32_t)UART1) & UART_TXFIFO_FULL);
}

uint8_t uart_available(void) 
{
    // Extract RX FIFO count from status register
    return (*(volatile uint32_t*)UART_STATUS_REG((uint32_t)UART1)) & UART_RXFIFO_CNT_MASK;
}