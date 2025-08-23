#include "ssm1.h"
#include "uart.h"
#include "defines.h"
#include "esp_log.h"

void ecu_init(void)
{
    static const uint8_t init_command[4] = {0x78, 0x00, 0x00, 0x00};
    uart_write_bytes(UART_NUM, init_command, sizeof(init_command));
    uart_wait_tx_done(UART_NUM, pdMS_TO_TICKS(50));
}

static inline void stop_read(void)
{
    static const uint8_t stop_command[4] = {0x12, 0x00, 0x00, 0x00};
    uart_write_bytes(UART_NUM, stop_command, sizeof(stop_command));
    uart_wait_tx_done(UART_NUM, pdMS_TO_TICKS(20));
}

bool get_romid(uint8_t *buffer)
{
    static const uint8_t romid_command[4] = {0x00, 0x46, 0x48, 0x49};
    uart_write_bytes(UART_NUM, romid_command, sizeof(romid_command));
    uart_wait_tx_done(UART_NUM, pdMS_TO_TICKS(10));

    int len = uart_read_bytes(UART_NUM, buffer, 3, pdMS_TO_TICKS(50));
    stop_read();

    if (len == 3 && buffer[0] != 0x00) {
        ESP_LOGI("ROMID", "Got ECU ROMID: %02X %02X %02X",
                 buffer[0], buffer[1], buffer[2]);
        return true;
    }

    return false;
}

uint8_t read_data_from_address(uint16_t addr)
{
    uint8_t read_command[4] = {0x78, (uint8_t)(GET_MSB(addr)), (uint8_t)(GET_LSB(addr)), 0x00};
    uint8_t answer[3] = {0};
    uart_write_bytes(UART_NUM, read_command, sizeof(read_command));
    uart_wait_tx_done(UART_NUM, pdMS_TO_TICKS(10));

    uart_read_bytes(UART_NUM, answer, 3, pdMS_TO_TICKS(50));
    stop_read();
    return answer[2]; // Return the data byte
}

void send_clear_command(uint16_t addr)
{
    uint8_t clear_command[4] = {0xAA, (uint8_t)(GET_MSB(addr)), (uint8_t)(GET_LSB(addr)), 0x00};
    uart_write_bytes(UART_NUM, clear_command, sizeof(clear_command));
    uart_wait_tx_done(UART_NUM, pdMS_TO_TICKS(10));
}
