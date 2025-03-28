#include <stdint.h>

#define UART0_BASE  0x3FF40000
#define UART1_BASE  0x3FF50000
#define UART2_BASE  0x3FF6E000

#define UART_CLK_FREQ 80000000
#define UART_SYS_CLK 160000000

#define UART_FIFO_REG(uart)  (uart + 0x0)    // Data register
#define UART_INT_RAW_REG(uart) (uart + 0x4)  // Interrupt status
#define UART_CONF0_REG(uart) (uart + 0x20)   // Control config
#define UART_CLKDIV_REG(uart) (uart + 0x14)  // Clock divider
#define UART_STATUS_REG(uart) (uart + 0x1C)  // UART Status Register

#define UART_TXFIFO_FULL  (1 << 21)         // TX FIFO Full Flag
#define UART_RXFIFO_CNT_MASK 0xFF

#define UART1  ((volatile uint32_t*) UART1_BASE) // Select UART1

#define UART_BAUD_RATE 1953
#define UART_CLK_DIV (UART_SYS_CLK / (UART_BAUD_RATE * 16))


void uart_init(void);
void uart_send_byte(uint8_t byte);
uint8_t uart_read_byte(void);
int uart_read_bytes(uint8_t* buf, int length);
void uart_flush(void);
uint8_t uart_available(void);