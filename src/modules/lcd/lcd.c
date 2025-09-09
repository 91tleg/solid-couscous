#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "state_machine.h"
#include "ssm1.h"
#include "assert_handler.h"
#include <stdbool.h>

#define I2C_MASTER_NUM          (I2C_NUM_0)
#define I2C_MASTER_SCL_IO       (GPIO_NUM_22)
#define I2C_MASTER_SDA_IO       (GPIO_NUM_21)
#define SLAVE_ADDRESS_LCD       (0x27U)
#define I2C_MASTER_FREQ_HZ      (100000U)

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
#define LCD_BUF_SIZE            (20)

#define SPLIT_NIBBLES(byte, high, low)                  \
    do {                                                \
        high = ((uint8_t)(byte)) & 0xF0U;               \
        low  = (((uint8_t)(byte) << 4) & 0xF0U);        \
    } while(0)

static esp_err_t err = ESP_OK;
static i2c_master_dev_handle_t dev_handle;
static i2c_master_bus_handle_t bus_handle;

static esp_err_t i2c_master_configure(void)
{
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_NUM,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true};
    err = i2c_new_master_bus(&i2c_mst_config, &bus_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE("LCD", "Failed to create i2c bus: %s", esp_err_to_name(err));
        return err;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SLAVE_ADDRESS_LCD,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ};
    err = i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE("LCD", "Failed to add device to bus: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI("LCD", "I2C master initialized...");
    return err;
}

bool i2c_initialized = false;
static void i2c_master_init(void)
{
    ASSERT(!i2c_initialized);
    i2c_master_configure();
    i2c_initialized = true;
}

static esp_err_t lcd_write_nibble(uint8_t nibble, bool is_data)
{
    uint8_t buf[1];
    uint8_t flags = LCD_BACKLIGHT | (is_data ? LCD_RS : 0);

    // EN = 1
    buf[0] = LCD_CMD_EN_UP(nibble | flags);
    if ((err = i2c_master_transmit(dev_handle, buf, 1, pdMS_TO_TICKS(LCD_I2C_TIMEOUT_MS))) != ESP_OK)
    {
        goto error;
    }

    // EN = 0
    buf[0] = LCD_CMD_EN_DOWN(nibble | flags);
    if ((err = i2c_master_transmit(dev_handle, buf, 1, pdMS_TO_TICKS(LCD_I2C_TIMEOUT_MS))) != ESP_OK)
    {
        goto error;
    }

    return ESP_OK;

error:
    ESP_LOGE("LCD", "Failed to send nibble: %s", esp_err_to_name(err));
    return err;
}

static esp_err_t lcd_send_command(uint8_t cmd)
{
    uint8_t high, low;
    SPLIT_NIBBLES(cmd, high, low);

    if ((err = lcd_write_nibble(high, false)) != ESP_OK)
    {
       return err;
    }
    if ((err = lcd_write_nibble(low, false)) != ESP_OK)
    {
        return err;
    }

    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_DELAY_MS));
    return ESP_OK;
}

static esp_err_t lcd_send_data(char data)
{
    uint8_t high, low;
    SPLIT_NIBBLES(data, high, low);

    if ((err = lcd_write_nibble(high, true)) != ESP_OK)
    {
        return err;
    }
    if ((err = lcd_write_nibble(low, true)) != ESP_OK)
    {
       return err; 
    }

    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_DELAY_MS));
    return ESP_OK;
}

static inline void lcd_clear(void)
{
    lcd_send_command(LCD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(LCD_CMD_INIT_DELAY_MS));
}

static void lcd_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t cmd = LCD_SET_CURSOR | (row == 1 ? LCD_ROW1_ADDR : LCD_ROW0_ADDR) | col;
    lcd_send_command(cmd);
}

static void lcd_init(void)
{
    i2c_master_init();
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

    ESP_LOGI("LCD", "LCD initialized...");
}

static void lcd_send_string(char *str)
{
    while (*str)
    {
        lcd_send_data((uint8_t)*str++);
        vTaskDelay(pdMS_TO_TICKS(LCD_CHAR_DELAY_MS));
    }
}

static void lcd_print_state(struct state_machine_data *data)
{
    static char lcd_buf[LCD_BUF_SIZE];
    switch (data->state)
    {
    case STATE_ROMID:
        ESP_LOGI("LCD", "romid");
        if (data->parameters.romid[0] == 0x00 &&
            data->parameters.romid[1] == 0x00 &&
            data->parameters.romid[2] == 0x00)
        {
            snprintf(lcd_buf, sizeof(lcd_buf), "ROM ID: --.--.--");
        }
        else
        {
            snprintf(lcd_buf, sizeof(lcd_buf), "ROM ID: %02X.%02X.%02X",
                     data->parameters.romid[0],
                     data->parameters.romid[1],
                     data->parameters.romid[2]);
        }
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        lcd_set_cursor(0, 1);
        lcd_send_string("                ");
        break;

    case STATE_BATTERY_V:
        ESP_LOGI("LCD", "battery");
        snprintf(lcd_buf, sizeof(lcd_buf), "VBAT: %.2fv     ", data->parameters.battery_voltage);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_VEHICLE_SPEED:
        ESP_LOGI("LCD", "speed");
        snprintf(lcd_buf, sizeof(lcd_buf), "VSPD: %dmph   ", data->parameters.vehicle_speed);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_ENGINE_SPEED:
        ESP_LOGI("LCD", "rpm");
        snprintf(lcd_buf, sizeof(lcd_buf), "REV: %drpm     ", data->parameters.engine_speed);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_COOLANT_TEMP:
        ESP_LOGI("LCD", "coolant");
        snprintf(lcd_buf, sizeof(lcd_buf), "WATR: %df      ", data->parameters.coolant_temp);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_AIRFLOW:
        ESP_LOGI("LCD", "maf");
        snprintf(lcd_buf, sizeof(lcd_buf), "MAF: %.2fv       ", data->parameters.airflow);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_THROTTLE:
        ESP_LOGI("LCD", "throttle");
        snprintf(lcd_buf, sizeof(lcd_buf), "TPS: %d%%       ", data->parameters.throttle_percentage);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_THROTTLE_V:
        ESP_LOGI("LCD", "throttle v");
        snprintf(lcd_buf, sizeof(lcd_buf), "THV: %.2fv      ", data->parameters.throttle_signal);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_MANIP:
        ESP_LOGI("LCD", "manip");
        snprintf(lcd_buf, sizeof(lcd_buf),
                 data->parameters.manip < 0 ? "MANIP: %.2finHg " : "MANIP: %.2fpsi  ",
                 data->parameters.manip < 0 ? -data->parameters.manip : data->parameters.manip);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_BOOST_SOLINOID:
        ESP_LOGI("LCD", "wgc");
        snprintf(lcd_buf, sizeof(lcd_buf), "WGC: %.2f%%      ", data->parameters.boost_solenoid);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_IGNITION_TIMING:
        ESP_LOGI("LCD", "ign");
        snprintf(lcd_buf, sizeof(lcd_buf), "IGN: %dBTDC    ", data->parameters.ignition_timing);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_LOAD:
        ESP_LOGI("LCD", "load");
        snprintf(lcd_buf, sizeof(lcd_buf), "LOAD: %d       ", data->parameters.engine_load);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_INJECTOR_PW:
        ESP_LOGI("LCD", "inj");
        snprintf(lcd_buf, sizeof(lcd_buf), "IPW: %.3fms   ", data->parameters.injector_pw);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_IAC:
        ESP_LOGI("LCD", "iac");
        snprintf(lcd_buf, sizeof(lcd_buf), "IAC: %.1f        ", data->parameters.iac);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_O2_V:
        ESP_LOGI("LCD", "o2");
        snprintf(lcd_buf, sizeof(lcd_buf), "O2: %.2fv     ", data->parameters.o2_signal);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_TIMING_CORRECTION:
        ESP_LOGI("LCD", "timing");
        snprintf(lcd_buf, sizeof(lcd_buf), "CORR: %d       ", data->parameters.timing_correction);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_FUEL_TRIM:
        ESP_LOGI("LCD", "trim");
        snprintf(lcd_buf, sizeof(lcd_buf), "TRIM: %.2f      ", data->parameters.fuel_trim);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_BAROP:
        ESP_LOGI("LCD", "barop");
        snprintf(lcd_buf, sizeof(lcd_buf), "BARO: %.2ftorr   ", data->parameters.barop);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_INPUT_SWITCHES:
        ESP_LOGI("LCD", "input switch");
        snprintf(lcd_buf, sizeof(lcd_buf), "IG%d AT%d TM%d RM%d ",
                 data->status0.ignition, data->status0.auto_trans, data->status0.test_mode, data->status0.read_mode);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "NT%d PK%d CA%d     ",
                 data->status0.neutral, data->status0.park, data->status0.california);
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf);
        break;

    case STATE_INOUT_SWITCHES:
        ESP_LOGI("LCD", "io switch");
        snprintf(lcd_buf, sizeof(lcd_buf), "ID%d AC%d AR%d RF%d ",
                 data->status1.idle_sw, data->status1.ac_sw, data->status1.ac_relay, data->status1.rad_fan);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "FP%d CN%d KS%d PX%d ",
                 data->status1.fuel_pump, data->status1.purge_valve, data->status1.pinging, data->status1.press_exch);
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf);
        break;

    case STATE_STORED_CODE_ONE:
    case STATE_ACTIVE_CODE_ONE:
        ESP_LOGI("LCD", "code 1");
        snprintf(lcd_buf, sizeof(lcd_buf), "11%d 12%d 13%d 14%d ",
                 data->status2.crank, data->status2.starter, data->status2.cam, data->status2.inj_1);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "15%d 16%d 17%d     ",
                 data->status2.inj_2, data->status2.inj_3, data->status2.inj_4);
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf);
        break;

    case STATE_STORED_CODE_TWO:
    case STATE_ACTIVE_CODE_TWO:
        ESP_LOGI("LCD", "code 2");
        snprintf(lcd_buf, sizeof(lcd_buf), "21%d 22%d 23%d 24%d ",
                 data->status3.temp, data->status3.knock, data->status3.maf, data->status3.iacv);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "31%d 32%d 33%d 35%d ",
                 data->status3.tps, data->status3.oxygen, data->status3.vss, data->status3.purge);
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf);
        break;

    case STATE_STORED_CODE_THREE:
    case STATE_ACTIVE_CODE_THREE:
        ESP_LOGI("LCD", "code 3");
        snprintf(lcd_buf, sizeof(lcd_buf), "41%d 42%d 44%d 45%d ",
                 data->status4.fuel_trim, data->status4.idle_sw, data->status4.wgc, data->status4.baro);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "49%d 51%d 52%d     ",
                 data->status4.wrong_maf, data->status4.neutral_sw, data->status4.parking_sw);
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf);
        break;
    }
}

void lcd_task(void *parameters)
{
    lcd_init();
    struct state_machine_data lcd_data;
    for (;;)
    {
        if (xQueueReceive(lcd_queue, &lcd_data, portMAX_DELAY) == pdTRUE)
        {
            lcd_print_state(&lcd_data);
        }
        vTaskDelay(pdMS_TO_TICKS(90));
    }
}