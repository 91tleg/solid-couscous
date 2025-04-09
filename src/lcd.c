#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "state_machine.h"
#include "ssm1.h"

#define I2C_NUM                     I2C_NUM_0
#define I2C_MASTER_SCL_IO           GPIO_NUM_22
#define I2C_MASTER_SDA_IO           GPIO_NUM_21
#define SLAVE_ADDRESS_LCD           (0x27)
#define I2C_MASTER_FREQ_HZ          (400000)
#define I2C_MASTER_NUM              (0)
#define I2C_MASTER_TX_BUF_DISABLE   (0)
#define I2C_MASTER_RX_BUF_DISABLE   (0)
#define I2C_MASTER_TIMEOUT_MS       (1000)

#define LCD_CMD_CLEAR_DISPLAY       (0x01)
#define LCD_CMD_RETURN_HOME         (0x02)
#define LCD_CMD_ENTRY_MODE_SET      (0x06)
#define LCD_CMD_DISPLAY_ON          (0x0C)
#define LCD_CMD_DISPLAY_OFF         (0x08)
#define LCD_CMD_FUNCTION_SET        (0x28)
#define LCD_CMD_SET_CURSOR          (0x80)
#define LCD_CMD_INIT_8_BIT_MODE     (0x30)
#define LCD_CMD_INIT_4_BIT_MODE     (0x20)

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

static void lcd_send_command(char cmd)
{
    char data_u, data_l;
    uint8_t data_t[4];

    data_u = (cmd & 0xf0);        // Upper nibble of the command
    data_l = ((cmd << 4) & 0xf0); // Lower nibble of the command
    data_t[0] = data_u | 0x0C;    // EN = 1, RS = 0
    data_t[1] = data_u | 0x08;    // EN = 0, RS = 0
    data_t[2] = data_l | 0x0C;    // EN = 1, RS = 0
    data_t[3] = data_l | 0x08;    // EN = 0, RS = 0

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
    data_t[0] = data_u | 0x0D;     // EN = 1, RS = 1
    data_t[1] = data_u | 0x09;     // EN = 0, RS = 1
    data_t[2] = data_l | 0x0D;     // EN = 1, RS = 1
    data_t[3] = data_l | 0x09;     // EN = 0, RS = 1

    err = i2c_master_write_to_device(I2C_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);

    if (err != 0)
    {
        ESP_LOGI("LCD", "Error sending data");
    }
}

static inline void lcd_clear(void)
{
    lcd_send_command(LCD_CMD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(5));
}

static void lcd_set_cursor(int row, int col)
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

    lcd_send_command(col); // Send command to set cursor position
}

static void lcd_init(void)
{
    i2c_master_init();

    // 4-bit initialization sequence
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_command(LCD_CMD_INIT_8_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_command(LCD_CMD_INIT_8_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(0.2));
    lcd_send_command(LCD_CMD_INIT_8_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(10));
    lcd_send_command(LCD_CMD_INIT_4_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(10));

    // Display initialization
    lcd_send_command(LCD_CMD_FUNCTION_SET); // Function set: 4-bit mode, 2-line display, 5x8 characters
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_command(LCD_CMD_DISPLAY_OFF);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_command(LCD_CMD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_command(LCD_CMD_ENTRY_MODE_SET); // Entry mode set: increment cursor, no shift
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send_command(LCD_CMD_DISPLAY_ON);
    vTaskDelay(pdMS_TO_TICKS(1));
}

static void lcd_send_string(char *str)
{
    while (*str)
        lcd_send_data(*str++);
}

static void print_romid(struct state_machine_data *data)
{
    static uint8_t buf[8];
    for (uint8_t i = 0; i < 8; ++i)
    {
        if (i == 2 || i == 5)
        {
            lcd_send_string(".");
        }
        if (data->parameters.romid[i] < 0x10)
        {
            lcd_send_string("0"); // Print leading zero for single-digit hex values
        }
        // lcd.print(answer[i], HEX);
    }
}

void lcd_task(void *parameters)
{
    lcd_init();
    lcd_send_string("hello");
    struct state_machine_data received_data;
    char lcd_buffer[16];

    for (;;)
    {
        if (xQueueReceive(lcd_queue, (void *)&received_data, portMAX_DELAY) == pdTRUE)
        {
            lcd_clear();
            switch (received_data.state)
            {
            case STATE_ROMID:
                ESP_LOGI("STATE", "romid");
                uint8_t romid_buf[3];
                get_romid(romid_buf);
                // lcd_send_string(romid_buf);
                break;
            case STATE_BATTERY_V:
                ESP_LOGI("STATE", "battery"); 
                break;
            case STATE_VEHICLE_SPEED:
                ESP_LOGI("STATE", "speed");
                snprintf(lcd_buffer, sizeof(lcd_buffer), "VSPD: %d km/h", received_data.parameters.vehicle_speed);
                lcd_set_cursor(0, 0);
                lcd_send_string(lcd_buffer);
                break;
            case STATE_ENGINE_SPEED:
                ESP_LOGI("STATE", "rpm");
                snprintf(lcd_buffer, sizeof(lcd_buffer), "REV: %d rpm", received_data.parameters.engine_speed);
                lcd_set_cursor(0, 0);
                lcd_send_string(lcd_buffer);
                break;
            case STATE_COOLANT_TEMP:
                break;
            case STATE_AIRFLOW:
                break;
            case STATE_THROTTLE:
                break;
            case STATE_THROTTLE_V:
                break;
            case STATE_MANIP:
                break;
            case STATE_BOOST_SOLINOID:
                break;
            case STATE_IGNITION_TIMING:
                break;
            case STATE_LOAD:

                break;
            case STATE_INJECTOR_PW:

                break;
            case STATE_IAC:

                break;
            case STATE_O2_V:

                break;
            case STATE_TIMING_CORRECTION:
            
                break;
            case STATE_FUEL_TRIM:

                break;
            case STATE_BAROP:

                break;
            case STATE_INPUT_SWITCHES:

                break;
            case STATE_INOUT_SWITCHES:

                break;
            case STATE_ACTIVE_CODE_ONE:

                break;
            case STATE_ACTIVE_CODE_TWO:

                break;
            case STATE_ACTIVE_CODE_THREE:

                break;
            case STATE_STORED_CODE_ONE:

                break;
            case STATE_STORED_CODE_TWO:

                break;
            case STATE_STORED_CODE_THREE:
                break;
            }
        }
    }
}