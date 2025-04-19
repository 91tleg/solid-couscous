#include "ssm1.h"
#include "uart.h"

#define GET_MSB(addr) ((addr) >> 8)
#define GET_LSB(addr) ((addr) & 0xFF)
#define MSB(addr) GET_MSB(addr)
#define LSB(addr) GET_LSB(addr)

static inline void stop_read(void)
{
    static const uint8_t stop_command[4] = {0x12, 0x00, 0x00, 0x00};
    uart_write_bytes(UART_NUM, (const char *)stop_command, sizeof(stop_command));
    uart_flush(UART_NUM);
}

void get_romid(uint8_t *buffer)
{
    static uint8_t romid[3] = {0};
    static const uint8_t read_command[4] = {0x78, 0x00, 0x00, 0x00};
    static const uint8_t romid_command[4] = {0x00, 0x46, 0x48, 0x49};

    uart_write_bytes(UART_NUM, (const char *)read_command, sizeof(read_command));
    uart_flush(UART_NUM);

    uart_write_bytes(UART_NUM, (const char *)romid_command, sizeof(romid_command));
    uart_flush(UART_NUM);

    int bytes_read = 0;
    while (bytes_read < 3)
    {
        int len = uart_read_bytes(UART_NUM, &romid[bytes_read], 1, pdMS_TO_TICKS(100));
        if (len > 0)
        {
            ++bytes_read;
        }
    }
    // Copy received ROM ID into buffer
    for (uint8_t i = 0; i < 3; ++i)
    {
        buffer[i] = romid[i];
    }
    stop_read();
}

__attribute__((weak)) uint8_t read_data_from_address(uint16_t addr)
{
    uint8_t read_command[4] = {0x78, (uint8_t)(MSB(addr)), (uint8_t)(LSB(addr)), 0x00};
    static uint8_t answer[3] = {0};

    uart_write_bytes(UART_NUM, (const char *)read_command, sizeof(read_command));
    uart_flush(UART_NUM);

    int retries = 0;
    while (retries < 3)
    {
        // Read 3 bytes from UART
        int num_bytes = uart_read_bytes(UART_NUM, answer, 3, pdMS_TO_TICKS(100));
        if ((num_bytes == 3) && (answer[0] == read_command[1]) && (answer[1] == read_command[2]))
        {
            break; // Valid response received
        }
        ++retries;
    }
    stop_read();
    return answer[2]; // Return the data byte
}

void send_clear_command(uint16_t addr)
{
    uint8_t clear_command[4] = {0xAA, (uint8_t)(MSB(addr)), (uint8_t)(LSB(addr)), 0x00};
    uart_write_bytes(UART_NUM, (const char *)clear_command, sizeof(clear_command));
    uart_flush(UART_NUM);
}
