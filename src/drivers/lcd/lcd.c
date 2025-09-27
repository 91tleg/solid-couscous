#include "lcd.h"
#include "drivers/i2c/i2c.h"
#include "core/log/log.h"
#include <stdbool.h>
#include <esp_err.h>
#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define TAG                     "Lcd"
#define LCD_CLEAR_DISPLAY       (0x01U)
#define LCD_RETURN_HOME         (0x02U)
#define LCD_ENTRY_MODE_SET      (0x06U)
#define LCD_DISPLAY_ON          (0x0CU)
#define LCD_DISPLAY_OFF         (0x08U)
#define LCD_FUNCTION_SET        (0x28U)
#define LCD_SET_CURSOR          (0x80U)
#define LCD_INIT_8_BIT_MODE     (0x30U)
#define LCD_INIT_4_BIT_MODE     (0x20U)
#define LCD_ROW0_ADDR           (0x00U)
#define LCD_ROW1_ADDR           (0x40U)

#define LCD_EN                  (0x04U)
#define LCD_RS                  (0x01U)
#define LCD_BACKLIGHT           (0x08U)
#define LCD_CMD_EN_UP(x)        ((x) | LCD_EN)
#define LCD_CMD_EN_DOWN(x)      ((x) & ~LCD_EN)
#define LCD_DATA(x)             ((x) | LCD_RS)

#define LCD_CMD_INIT_DELAY_MS   (5U)
#define LCD_CMD_DELAY_MS        (5U)
#define LCD_POWER_ON_DELAY_MS   (2000U)
#define LCD_CHAR_DELAY_MS       (1U)
#define LCD_I2C_TIMEOUT_MS      (1000U)

#define SPLIT_NIBBLES(byte, high, low)                  \
    do {                                                \
        high = ((uint8_t)(byte)) & 0xF0U;               \
        low  = (((uint8_t)(byte) << 4) & 0xF0U);        \
    } while(0)

static esp_err_t lcd_write_nibble(uint8_t nibble, bool is_data)
{
    i2c_master_dev_handle_t lcd_handle = i2c_master_get_device();
    esp_err_t err = ESP_OK;
    uint8_t buf[1];
    uint8_t flags = LCD_BACKLIGHT | (is_data ? LCD_RS : 0);

    // EN = 1
    buf[0] = LCD_CMD_EN_UP(nibble | flags);
    err = i2c_master_transmit(lcd_handle, buf, 1, pdMS_TO_TICKS(LCD_I2C_TIMEOUT_MS));
    if (err != ESP_OK)
    {
        goto error;
    }

    // EN = 0
    buf[0] = LCD_CMD_EN_DOWN(nibble | flags);
    err = i2c_master_transmit(lcd_handle, buf, 1, pdMS_TO_TICKS(LCD_I2C_TIMEOUT_MS));
    if (err != ESP_OK)
    {
        goto error;
    }

    return err;

error:
    LOGE(TAG, "Failed to send nibble: %s", esp_err_to_name(err));
    return err;
}

static esp_err_t lcd_send_command(uint8_t cmd)
{
    esp_err_t err = ESP_OK;
    uint8_t high;
    uint8_t low;
    SPLIT_NIBBLES(cmd, high, low);

    err = lcd_write_nibble(high, false);
    if (err != ESP_OK)
    {
       goto error;
    }
    err = lcd_write_nibble(low, false);
    if (err != ESP_OK)
    {
        goto error;
    }

    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_DELAY_MS));
    return err;

error:
    LOGE(TAG, "Failed to send command: %s", esp_err_to_name(err));
    return err;
}

static esp_err_t lcd_send_data(char data)
{
    esp_err_t err = ESP_OK;
    uint8_t high;
    uint8_t low;
    SPLIT_NIBBLES(data, high, low);

    err = lcd_write_nibble(high, true);
    if (err != ESP_OK)
    {
        goto error;
    }
    err = lcd_write_nibble(low, true);
    if (err != ESP_OK)
    {
       goto error;
    }

    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_DELAY_MS));
    return err;

error:
    LOGE(TAG, "Failed to send data: %s", esp_err_to_name(err));
    return err;
}

void lcd_driver_init(void)
{
    vTaskDelay(pdMS_TO_TICKS(LCD_POWER_ON_DELAY_MS));

    lcd_write_nibble(LCD_INIT_8_BIT_MODE, false);
    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_INIT_DELAY_MS));

    lcd_write_nibble(LCD_INIT_8_BIT_MODE, false);
    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_INIT_DELAY_MS));

    lcd_write_nibble(LCD_INIT_8_BIT_MODE, false);
    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_INIT_DELAY_MS));

    // 4-bit mode
    lcd_write_nibble(LCD_INIT_4_BIT_MODE, false);
    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_INIT_DELAY_MS));

    lcd_send_command(LCD_FUNCTION_SET);
    lcd_send_command(LCD_DISPLAY_OFF);
    lcd_clear();
    lcd_send_command(LCD_ENTRY_MODE_SET);
    lcd_send_command(LCD_DISPLAY_ON);
}

void lcd_clear(void)
{
    lcd_send_command(LCD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_INIT_DELAY_MS));
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t cmd = LCD_SET_CURSOR | (row == 1 ? LCD_ROW1_ADDR : LCD_ROW0_ADDR) | col;
    lcd_send_command(cmd);
}

void lcd_print(char *str)
{
    while (*str)
    {
        lcd_send_data((uint8_t)*str++);
        vTaskDelay(pdMS_TO_TICKS(LCD_CHAR_DELAY_MS));
    }
}