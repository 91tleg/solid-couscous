#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "state_machine.h"
#include "ssm1.h"

#define I2C_NUM                     (I2C_NUM_0)
#define I2C_MASTER_SCL_IO           (GPIO_NUM_22)
#define I2C_MASTER_SDA_IO           (GPIO_NUM_21)
#define SLAVE_ADDRESS_LCD           (0x27U)
#define I2C_MASTER_FREQ_HZ          (400000U)
#define I2C_MASTER_NUM              (0)

#define LCD_CMD_CLEAR_DISPLAY       (0x01U)
#define LCD_CMD_RETURN_HOME         (0x02U)
#define LCD_CMD_ENTRY_MODE_SET      (0x06U)
#define LCD_CMD_DISPLAY_ON          (0x0CU)
#define LCD_CMD_DISPLAY_OFF         (0x08U)
#define LCD_CMD_FUNCTION_SET        (0x28U)
#define LCD_CMD_SET_CURSOR          (0x80U)
#define LCD_CMD_INIT_8_BIT_MODE     (0x30U)
#define LCD_CMD_INIT_4_BIT_MODE     (0x20U)

#define BIT_MASK (0xF0U)
#define GET_UPPER_NIBBLE(x) ((x) & BIT_MASK)
#define GET_LOWER_NIBBLE_SHIFTED(x) (((x) << 4) & BIT_MASK)

esp_err_t err;
static i2c_master_dev_handle_t dev_handle;
static i2c_master_bus_handle_t bus_handle;

static esp_err_t i2c_master_init(void)
{
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_NUM,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SLAVE_ADDRESS_LCD,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
    ESP_LOGI("LCD", "I2C master initialized...");
    return ESP_OK;
}

static void lcd_send_command(uint8_t cmd)
{
    static uint8_t data_u, data_l;
    static uint8_t buf[4];

    data_u = GET_UPPER_NIBBLE(cmd);
    data_l = GET_LOWER_NIBBLE_SHIFTED(cmd);
    buf[0] = data_u | 0x0C;    // EN = 1, RS = 0
    buf[1] = data_u | 0x08;    // EN = 0, RS = 0
    buf[2] = data_l | 0x0C;    // EN = 1, RS = 0
    buf[3] = data_l | 0x08;    // EN = 0, RS = 0
    err = i2c_master_transmit(dev_handle, buf, 4, pdMS_TO_TICKS(1000));
}

static void lcd_send_data(char data)
{
    static char data_u, data_l;
    static uint8_t buf[4];

    data_u = GET_UPPER_NIBBLE(data);
    data_l = GET_LOWER_NIBBLE_SHIFTED(data);
    buf[0] = data_u | 0x0D;     // EN = 1, RS = 1
    buf[1] = data_u | 0x09;     // EN = 0, RS = 1
    buf[2] = data_l | 0x0D;     // EN = 1, RS = 1
    buf[3] = data_l | 0x09;     // EN = 0, RS = 1
    err = i2c_master_transmit(dev_handle, buf, 4, pdMS_TO_TICKS(1000));
}

static inline void lcd_clear(void)
{
    lcd_send_command(LCD_CMD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(5));
}

static void lcd_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t cmd = LCD_CMD_SET_CURSOR;
    if (row == 1)
    {
        cmd |= 0x40;
    }
    cmd |= col;
    lcd_send_command(cmd);
}

static void lcd_init(void)
{
    i2c_master_init();

    vTaskDelay(pdMS_TO_TICKS(1050));
    lcd_send_command(LCD_CMD_INIT_8_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_command(LCD_CMD_INIT_8_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_command(LCD_CMD_INIT_8_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_command(LCD_CMD_INIT_4_BIT_MODE);
    vTaskDelay(pdMS_TO_TICKS(5));

    // Display initialization
    lcd_send_command(LCD_CMD_FUNCTION_SET);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_command(LCD_CMD_DISPLAY_OFF);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_command(LCD_CMD_CLEAR_DISPLAY);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_command(LCD_CMD_ENTRY_MODE_SET);
    vTaskDelay(pdMS_TO_TICKS(5));
    lcd_send_command(LCD_CMD_DISPLAY_ON);
    vTaskDelay(pdMS_TO_TICKS(5));
    ESP_LOGI("LCD", "LCD initialized...");
}

static void lcd_send_string(char *str)
{
    while (*str)
    {
        lcd_send_data(*str++);
    }
}

static void lcd_print_state(struct state_machine_data *data)
{
    static char lcd_buf[20];
    switch (data->state)
    {
    case STATE_ROMID:
        ESP_LOGI("LCD", "romid");
        snprintf(lcd_buf, sizeof(lcd_buf), "ROM ID: %02X.%02X.%02X ", data->parameters.romid[0], data->parameters.romid[1], data->parameters.romid[2]);
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
        snprintf(lcd_buf, sizeof(lcd_buf), "VSPD: %.0fmph   ", data->parameters.vehicle_speed);
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
        snprintf(lcd_buf, sizeof(lcd_buf), "IAC: %d        ", data->parameters.iac);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_O2_V:
        ESP_LOGI("LCD", "o2");
        snprintf(lcd_buf, sizeof(lcd_buf), "O2: %.2f      ", data->parameters.o2_signal);
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
        snprintf(lcd_buf, sizeof(lcd_buf), "IGN: %.2ftorr   ", data->parameters.barop);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        break;

    case STATE_INPUT_SWITCHES:
        ESP_LOGI("LCD", "input switch");
        snprintf(lcd_buf, sizeof(lcd_buf), "IG%d AT%d TM%d RM%d ", data->status0.ignition, data->status0.auto_trans, data->status0.test_mode, data->status0.read_mode);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "NT%d PK%d CA%d     ", data->status0.neutral, data->status0.park, data->status0.california);               
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf); 
        break;

    case STATE_INOUT_SWITCHES:
        ESP_LOGI("LCD", "io switch");
        snprintf(lcd_buf, sizeof(lcd_buf), "ID%d AC%d AR%d RF%d ", data->status1.idle_sw, data->status1.ac_sw, data->status1.ac_relay, data->status1.rad_fan);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "FP%d CN%d KS%d PX%d ", data->status1.fuel_pump, data->status1.purge_valve, data->status1.pinging, data->status1.press_exch);
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf);
        break;

    case STATE_STORED_CODE_ONE:
    case STATE_ACTIVE_CODE_ONE:
        ESP_LOGI("LCD", "code 1");
        snprintf(lcd_buf, sizeof(lcd_buf), "11%d 12%d 13%d 14%d ", data->status2.crank, data->status2.starter, data->status2.cam, data->status2.inj_1);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "15%d 16%d 17%d     ", data->status2.inj_2, data->status2.inj_3, data->status2.inj_4);
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf);
        break;
    
    case STATE_STORED_CODE_TWO:
    case STATE_ACTIVE_CODE_TWO:
        ESP_LOGI("LCD", "code 2");
        snprintf(lcd_buf, sizeof(lcd_buf), "21%d 22%d 23%d 24%d ", data->status3.temp, data->status3.knock, data->status3.maf, data->status3.iacv);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "31%d 32%d 33%d 35%d ", data->status3.tps, data->status3.oxygen, data->status3.vss, data->status3.purge);
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf);
        break;

    case STATE_STORED_CODE_THREE:
    case STATE_ACTIVE_CODE_THREE:
        ESP_LOGI("LCD", "code 3");
        snprintf(lcd_buf, sizeof(lcd_buf), "41%d 42%d 44%d 45%d ", data->status4.fuel_trim, data->status4.idle_sw, data->status4.wgc, data->status4.baro);
        lcd_set_cursor(0, 0);
        lcd_send_string(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "49%d 51%d 52%d     ", data->status4.wrong_maf, data->status4.neutral_sw, data->status4.parking_sw);
        lcd_set_cursor(0, 1);
        lcd_send_string(lcd_buf);
        break;
    }
}

void lcd_task(void *parameters)
{
    lcd_init();
    static struct state_machine_data data;
    for (;;)
    {
        if (xQueueReceive(lcd_queue, (void *)&data, portMAX_DELAY) == pdTRUE)
        {
            lcd_print_state(&data);
        }
        vTaskDelay(pdMS_TO_TICKS(90));
    }
}