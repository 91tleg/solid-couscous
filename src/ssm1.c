#include "ssm1.h"

#include <stdint.h>

void stop_read(void)
{
    uint8_t txbuf[4] = {0x12, 0x00, 0x00, 0x00};
    for (uint8_t i = 0; i < 4; ++i)
    {
        uart_send_byte(txbuf[i]);
    }
    uart_flush();
}

void get_romid(uint8_t *buffer)
{
    uint8_t read_command[4] = {0x78, 0x00, 0x00, 0x00};
    uint8_t romid_command[4] = {0x00, 0x46, 0x48, 0x49};
    uint8_t romid[3] = {0};
    for (uint8_t i = 0; i < 4; ++i)
    {
        uart_send_byte(read_command[i]);
    }
    uart_flush();
    for (uint8_t i = 0; i < 4; ++i)
    {
        uart_send_byte(romid_command[i]);
    }
    uart_flush();

    int bytes_read = 0;
    while (bytes_read < 3)
    {
        if (uart_available())
        {
            romid[bytes_read] = uart_read_byte();
            ++bytes_read;
        }
    }
    for (uint8_t i = 0; i < 3; ++i)
    {
        buffer[i] = romid[i]; // Copy received ROM ID into buffer
    }
    stop_read();
}

uint8_t read_data_from_address(uint16_t addr)
{
    uint8_t read_command[4] = {0x78, (uint8_t)(addr >> 8), (uint8_t)(addr & 0xff), 0x00};
    uint8_t answer[3] = {0};
    for (uint8_t i = 0; i < 4; ++i)
    {
       uart_send_byte(read_command[i]);
    }
    uart_flush();

    int retries = 0;
    while (retries < 3)
    {
        if (uart_available() >= 3) // Ensure 3 bytes are available before reading
        {
            int nbytes = uart_read_bytes(answer, 3);
            if ((nbytes == 3) && (answer[0] == read_command[1]) && (answer[1] == read_command[2]))
                break; // Valid response received
        }
        ++retries;
    }
    stop_read();
    return answer[2]; // Return the data byte
}


void send_clear_command(uint16_t addr)
{
    uint8_t clear_command[4] = {0xAA, (uint8_t)(addr >> 8), (uint8_t)(addr & 0xff), 0x00};
    for (uint8_t i = 0; i < 4; ++i)
    {
        uart_send_byte(clear_command[i]);
    }
    uart_flush();
}