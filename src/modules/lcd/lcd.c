#include "lcd.h"
#include "drivers/lcd/lcd.h"
#include "modules/state_machine/state_machine.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <esp_log.h>

#define TAG          "LCD"
#define LCD_BUF_SIZE (20)

static void lcd_print_state(struct state_machine_data *data)
{
    static char lcd_buf[LCD_BUF_SIZE];
    switch (data->state)
    {
    case STATE_ROMID:
        ESP_LOGI(TAG, "romid");
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
        lcd_print(lcd_buf);
        lcd_set_cursor(0, 1);
        lcd_print("                ");
        break;

    case STATE_BATTERY_V:
        ESP_LOGI(TAG, "battery");
        snprintf(lcd_buf, sizeof(lcd_buf), "VBAT: %.2fv     ", data->parameters.battery_voltage);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_VEHICLE_SPEED:
        ESP_LOGI(TAG, "speed");
        snprintf(lcd_buf, sizeof(lcd_buf), "VSPD: %dmph   ", data->parameters.vehicle_speed);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_ENGINE_SPEED:
        ESP_LOGI(TAG, "rpm");
        snprintf(lcd_buf, sizeof(lcd_buf), "REV: %drpm     ", data->parameters.engine_speed);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_COOLANT_TEMP:
        ESP_LOGI(TAG, "coolant");
        snprintf(lcd_buf, sizeof(lcd_buf), "WATR: %df      ", data->parameters.coolant_temp);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_AIRFLOW:
        ESP_LOGI(TAG, "maf");
        snprintf(lcd_buf, sizeof(lcd_buf), "MAF: %.2fv       ", data->parameters.airflow);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_THROTTLE:
        ESP_LOGI(TAG, "throttle");
        snprintf(lcd_buf, sizeof(lcd_buf), "TPS: %d%%       ", data->parameters.throttle_percentage);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_THROTTLE_V:
        ESP_LOGI(TAG, "throttle v");
        snprintf(lcd_buf, sizeof(lcd_buf), "THV: %.2fv      ", data->parameters.throttle_signal);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_MANIP:
        ESP_LOGI(TAG, "manip");
        snprintf(lcd_buf, sizeof(lcd_buf),
                 data->parameters.manip < 0 ? "MANIP: %.2finHg " : "MANIP: %.2fpsi  ",
                 data->parameters.manip < 0 ? -data->parameters.manip : data->parameters.manip);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_BOOST_SOLINOID:
        ESP_LOGI(TAG, "wgc");
        snprintf(lcd_buf, sizeof(lcd_buf), "WGC: %.2f%%      ", data->parameters.boost_solenoid);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_IGNITION_TIMING:
        ESP_LOGI(TAG, "ign");
        snprintf(lcd_buf, sizeof(lcd_buf), "IGN: %dBTDC    ", data->parameters.ignition_timing);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_LOAD:
        ESP_LOGI(TAG, "load");
        snprintf(lcd_buf, sizeof(lcd_buf), "LOAD: %d       ", data->parameters.engine_load);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_INJECTOR_PW:
        ESP_LOGI(TAG, "inj");
        snprintf(lcd_buf, sizeof(lcd_buf), "IPW: %.3fms   ", data->parameters.injector_pw);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_IAC:
        ESP_LOGI(TAG, "iac");
        snprintf(lcd_buf, sizeof(lcd_buf), "IAC: %.1f        ", data->parameters.iac);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_O2_V:
        ESP_LOGI(TAG, "o2");
        snprintf(lcd_buf, sizeof(lcd_buf), "O2: %.2fv     ", data->parameters.o2_signal);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_TIMING_CORRECTION:
        ESP_LOGI(TAG, "timing");
        snprintf(lcd_buf, sizeof(lcd_buf), "CORR: %d       ", data->parameters.timing_correction);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_FUEL_TRIM:
        ESP_LOGI(TAG, "trim");
        snprintf(lcd_buf, sizeof(lcd_buf), "TRIM: %.2f      ", data->parameters.fuel_trim);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_BAROP:
        ESP_LOGI(TAG, "barop");
        snprintf(lcd_buf, sizeof(lcd_buf), "BARO: %.2ftorr   ", data->parameters.barop);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        break;

    case STATE_INPUT_SWITCHES:
        ESP_LOGI(TAG, "input switch");
        snprintf(lcd_buf, sizeof(lcd_buf), "IG%d AT%d TM%d RM%d ",
                 data->status0.ignition, data->status0.auto_trans, data->status0.test_mode, data->status0.read_mode);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "NT%d PK%d CA%d     ",
                 data->status0.neutral, data->status0.park, data->status0.california);
        lcd_set_cursor(0, 1);
        lcd_print(lcd_buf);
        break;

    case STATE_INOUT_SWITCHES:
        ESP_LOGI(TAG, "io switch");
        snprintf(lcd_buf, sizeof(lcd_buf), "ID%d AC%d AR%d RF%d ",
                 data->status1.idle_sw, data->status1.ac_sw, data->status1.ac_relay, data->status1.rad_fan);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "FP%d CN%d KS%d PX%d ",
                 data->status1.fuel_pump, data->status1.purge_valve, data->status1.pinging, data->status1.press_exch);
        lcd_set_cursor(0, 1);
        lcd_print(lcd_buf);
        break;

    case STATE_STORED_CODE_ONE:
    case STATE_ACTIVE_CODE_ONE:
        ESP_LOGI(TAG, "code 1");
        snprintf(lcd_buf, sizeof(lcd_buf), "11%d 12%d 13%d 14%d ",
                 data->status2.crank, data->status2.starter, data->status2.cam, data->status2.inj_1);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "15%d 16%d 17%d     ",
                 data->status2.inj_2, data->status2.inj_3, data->status2.inj_4);
        lcd_set_cursor(0, 1);
        lcd_print(lcd_buf);
        break;

    case STATE_STORED_CODE_TWO:
    case STATE_ACTIVE_CODE_TWO:
        ESP_LOGI(TAG, "code 2");
        snprintf(lcd_buf, sizeof(lcd_buf), "21%d 22%d 23%d 24%d ",
                 data->status3.temp, data->status3.knock, data->status3.maf, data->status3.iacv);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "31%d 32%d 33%d 35%d ",
                 data->status3.tps, data->status3.oxygen, data->status3.vss, data->status3.purge);
        lcd_set_cursor(0, 1);
        lcd_print(lcd_buf);
        break;

    case STATE_STORED_CODE_THREE:
    case STATE_ACTIVE_CODE_THREE:
        ESP_LOGI(TAG, "code 3");
        snprintf(lcd_buf, sizeof(lcd_buf), "41%d 42%d 44%d 45%d ",
                 data->status4.fuel_trim, data->status4.idle_sw, data->status4.wgc, data->status4.baro);
        lcd_set_cursor(0, 0);
        lcd_print(lcd_buf);
        snprintf(lcd_buf, sizeof(lcd_buf), "49%d 51%d 52%d     ",
                 data->status4.wrong_maf, data->status4.neutral_sw, data->status4.parking_sw);
        lcd_set_cursor(0, 1);
        lcd_print(lcd_buf);
        break;
    }
}

void lcd_task(void *parameters)
{
    lcd_init();
    struct state_machine_data lcd_data;
    QueueHandle_t lcd_queue = state_machine_get_lcd_queue();
    for (;;)
    {
        if (xQueueReceive(lcd_queue, &lcd_data, portMAX_DELAY) == pdTRUE)
        {
            lcd_print_state(&lcd_data);
        }
        vTaskDelay(pdMS_TO_TICKS(90));
    }
}