#include "hd44780.h"
#include "i2c.h"
#include "log.h"
#include <stdbool.h>
#include <esp_err.h>
#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TAG                 "Hd44780"
#define CLEAR_DISPLAY       (0x01U)
#define RETURN_HOME         (0x02U)
#define ENTRY_MODE_SET      (0x06U)
#define DISPLAY_ON          (0x0CU)
#define DISPLAY_OFF         (0x08U)
#define FUNCTION_SET        (0x28U)
#define SET_CURSOR          (0x80U)
#define INIT_8_BIT_MODE     (0x30U)
#define INIT_4_BIT_MODE     (0x20U)
#define ROW0_ADDR           (0x00U)
#define ROW1_ADDR           (0x40U)

#define EN                  (0x04U)
#define RS                  (0x01U)
#define BACKLIGHT           (0x08U)
#define CMD_EN_UP(x)        ((x) | EN)
#define CMD_EN_DOWN(x)      ((x) & ~EN)
#define DATA(x)             ((x) | RS)

#define INIT_DELAY_MS       (5U)
#define CMD_DELAY_MS        (5U)
#define POWER_ON_DELAY_MS   (2000U)
#define CHAR_DELAY_MS       (1U)
#define I2C_TIMEOUT_MS      (1000U)

#define SPLIT_NIBBLES(byte, high, low)                  \
    do {                                                \
        high = ((uint8_t)(byte)) & 0xF0U;               \
        low  = (((uint8_t)(byte) << 4U) & 0xF0U);       \
    } while(0)

static esp_err_t hd44780_write_nibble(uint8_t nibble, bool is_data)
{
    i2c_master_dev_handle_t i2c_handle = i2c_master_get_handle();
    esp_err_t err = ESP_OK;
    uint8_t buf[1];
    uint8_t flags = BACKLIGHT | (is_data ? RS : 0);

    // EN = 1
    buf[0] = CMD_EN_UP(nibble | flags);
    err = i2c_master_transmit(i2c_handle, buf, 1, pdMS_TO_TICKS(I2C_TIMEOUT_MS));
    if (err != ESP_OK)
    {
        goto error;
    }

    // EN = 0
    buf[0] = CMD_EN_DOWN(nibble | flags);
    err = i2c_master_transmit(i2c_handle, buf, 1, pdMS_TO_TICKS(I2C_TIMEOUT_MS));
    if (err != ESP_OK)
    {
        goto error;
    }

    return err;

error:
    LOGE(TAG, "Failed to send nibble: %s", esp_err_to_name(err));
    return err;
}

static esp_err_t hd44780_send_command(uint8_t cmd)
{
    esp_err_t err = ESP_OK;
    uint8_t high;
    uint8_t low;
    SPLIT_NIBBLES(cmd, high, low);

    err = hd44780_write_nibble(high, false);
    if (err != ESP_OK)
    {
       goto error;
    }
    err = hd44780_write_nibble(low, false);
    if (err != ESP_OK)
    {
        goto error;
    }

    vTaskDelay(pdMS_TO_TICKS(CMD_DELAY_MS));
    return err;

error:
    LOGE(TAG, "Failed to send command: %s", esp_err_to_name(err));
    return err;
}

static esp_err_t hd44780_send_data(char data)
{
    esp_err_t err = ESP_OK;
    uint8_t high;
    uint8_t low;
    SPLIT_NIBBLES(data, high, low);

    err = hd44780_write_nibble(high, true);
    if (err != ESP_OK)
    {
        goto error;
    }
    err = hd44780_write_nibble(low, true);
    if (err != ESP_OK)
    {
       goto error;
    }

    vTaskDelay(pdMS_TO_TICKS(CMD_DELAY_MS));
    return err;

error:
    LOGE(TAG, "Failed to send data: %s", esp_err_to_name(err));
    return err;
}

void hd44780_driver_init(void)
{
    vTaskDelay(pdMS_TO_TICKS(POWER_ON_DELAY_MS));

    hd44780_write_nibble(INIT_8_BIT_MODE, false);
    vTaskDelay(pdMS_TO_TICKS(INIT_DELAY_MS));

    hd44780_write_nibble(INIT_8_BIT_MODE, false);
    vTaskDelay(pdMS_TO_TICKS(INIT_DELAY_MS));

    hd44780_write_nibble(INIT_8_BIT_MODE, false);
    vTaskDelay(pdMS_TO_TICKS(INIT_DELAY_MS));

    hd44780_write_nibble(INIT_4_BIT_MODE, false);
    vTaskDelay(pdMS_TO_TICKS(INIT_DELAY_MS));

    hd44780_send_command(FUNCTION_SET);
    hd44780_send_command(DISPLAY_OFF);
    hd44780_clear();
    hd44780_send_command(ENTRY_MODE_SET);
    hd44780_send_command(DISPLAY_ON);
}

void hd44780_clear(void)
{
    hd44780_send_command(CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(CMD_DELAY_MS));
}

void hd44780_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t cmd =
        SET_CURSOR | (row == 1 ? ROW1_ADDR : ROW0_ADDR) | col;
    hd44780_send_command(cmd);
}

void hd44780_print(char *str)
{
    while (*str)
    {
        hd44780_send_data((uint8_t)*str++);
        vTaskDelay(pdMS_TO_TICKS(CHAR_DELAY_MS));
    }
}