#include "lcd.h"
#include "driver/i2c.h"
#include "esp_log.h"

#define SLAVE_ADDRESS_LCD (0x27)
#define I2C_MASTER_SCL_IO GPIO_NUM_22
#define I2C_MASTER_SDA_IO GPIO_NUM_21
#define I2C_MASTER_NUM (0)
#define I2C_MASTER_FREQ_HZ (400000)
#define I2C_MASTER_TX_BUF_DISABLE (0)
#define I2C_MASTER_RX_BUF_DISABLE (0)
#define I2C_MASTER_TIMEOUT_MS (1000)
#define I2C_NUM I2C_NUM_0

#define LCD_CMD_CLEAR_DISPLAY 0x01
#define LCD_CMD_RETURN_HOME 0x02
#define LCD_CMD_ENTRY_MODE_SET 0x06
#define LCD_CMD_DISPLAY_ON 0x0C
#define LCD_CMD_DISPLAY_OFF 0x08
#define LCD_CMD_FUNCTION_SET 0x28
#define LCD_CMD_SET_CURSOR 0x80
#define LCD_CMD_INIT_8_BIT_MODE 0x30
#define LCD_CMD_INIT_4_BIT_MODE 0x20

esp_err_t err;

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

static void lcd_send_cmd(char cmd)
{
    char data_u, data_l;
    uint8_t data_t[4];

    data_u = (cmd & 0xf0);        // Upper nibble of the command
    data_l = ((cmd << 4) & 0xf0); // Lower nibble of the command
    data_t[0] = data_u | 0x0C; // EN = 1, RS = 0
    data_t[1] = data_u | 0x08; // EN = 0, RS = 0
    data_t[2] = data_l | 0x0C; // EN = 1, RS = 0
    data_t[3] = data_l | 0x08; // EN = 0, RS = 0

    err = i2c_master_write_to_device(I2C_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);

    if (err != 0)
    {
        ESP_LOGI("LCD", "Error sending command");
    }
}

static void lcd_send_data(char data)
{
    char data_u, data_l;
    uint8_t data_t[4];

    data_u = (data & 0xf0);        // Upper nibble of the data
    data_l = ((data << 4) & 0xf0); // Lower nibble of the data
    data_t[0] = data_u | 0x0D; // EN = 1, RS = 1
    data_t[1] = data_u | 0x09; // EN = 0, RS = 1
    data_t[2] = data_l | 0x0D; // EN = 1, RS = 1
    data_t[3] = data_l | 0x09; // EN = 0, RS = 1

    err = i2c_master_write_to_device(I2C_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);

    if (err != 0)
    {
        ESP_LOGI("LCD", "Error sending data");
    }
}

static inline void lcd_clear(void)
{
    lcd_send_cmd(LCD_CMD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(5));
}

static void lcd_put_cursor(int row, int col)
{
    switch (row)
    {
    case 0:
        col |= LCD_CMD_SET_CURSOR; // Set position for row 0
        break;
    case 1:
        col |= (LCD_CMD_SET_CURSOR | 0x40); // Set position for row 1
        break;
    }

    lcd_send_cmd(col); // Send command to set cursor position
}

static void lcd_init(void)
{
    i2c_master_init();

    // 4-bit initialization sequence
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_cmd(LCD_CMD_INIT_8_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_cmd(LCD_CMD_INIT_8_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(0.2));
    lcd_send_cmd(LCD_CMD_INIT_8_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(10));
    lcd_send_cmd(LCD_CMD_INIT_4_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(10));

    // Display initialization
    lcd_send_cmd(LCD_CMD_FUNCTION_SET); // Function set: 4-bit mode, 2-line display, 5x8 characters
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_cmd(LCD_CMD_DISPLAY_OFF);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_cmd(LCD_CMD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_cmd(LCD_CMD_ENTRY_MODE_SET); // Entry mode set: increment cursor, no shift
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_cmd(LCD_CMD_DISPLAY_ON);
    vTaskDelay(pdMS_TO_TICKS(1));
}

static void lcd_send_string(char *str)
{
    while (*str)
        lcd_send_data(*str++);
}

void lcd_task(void *pvParameters)
{
    lcd_init();
    lcd_put_cursor(0, 0);
    lcd_send_string("Hello world!");

    for(;;)
    {

    }
}