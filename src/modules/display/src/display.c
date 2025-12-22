#include "display.h"
#include "hd44780.h"
#include "log.h"
#include "fsm_data.h"
#include <stdio.h>
#include <string.h>

#define TAG "Display"

void display_print_state(struct fsm_data *data)
{
    static char buf[20];
    switch (data->state)
    {
    case STATE_ROMID:
        LOGI(TAG, "romid");
        if ((data->params.romid[0]
             | data->params.romid[1]
             | data->params.romid[2]) == 0x00)
        {
            strcpy(buf, "ROM ID: --.--.--   ");
        } else {
            snprintf(buf, sizeof(buf), "ROM ID: %02X.%02X.%02X",
                     data->params.romid[0],
                     data->params.romid[1],
                     data->params.romid[2]);
        }
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        hd44780_set_cursor(0, 1);
        hd44780_print("                ");
        break;

    case STATE_BATTERY_V:
        LOGI(TAG, "battery");
        snprintf(buf, sizeof(buf), "VBAT: %.2fv     ",
                 data->params.battery_voltage);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_VEHICLE_SPEED:
        LOGI(TAG, "speed");
        snprintf(buf, sizeof(buf), "VSPD: %dmph   ",
                 data->params.vehicle_speed);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_ENGINE_SPEED:
        LOGI(TAG, "rpm");
        snprintf(buf, sizeof(buf), "REV: %drpm     ",
                 data->params.engine_speed);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_COOLANT_TEMP:
        LOGI(TAG, "coolant");
        snprintf(buf, sizeof(buf), "WATR: %df      ",
                 data->params.coolant_temp);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_AIRFLOW:
        LOGI(TAG, "maf");
        snprintf(buf, sizeof(buf), "MAF: %.2fv       ",
                 data->params.airflow);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_THROTTLE:
        LOGI(TAG, "throttle");
        snprintf(buf, sizeof(buf), "TPS: %d%%       ",
                 data->params.throttle_percentage);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_THROTTLE_V:
        LOGI(TAG, "throttle v");
        snprintf(buf, sizeof(buf), "THV: %.2fv      ",
                 data->params.throttle_signal);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_MANIP:
        LOGI(TAG, "manip");
        snprintf(buf, sizeof(buf), data->params.manip < 0
                                   ? "MANIP: %.2finHg "
                                   : "MANIP: %.2fpsi  ",
                                   data->params.manip < 0
                                   ? -data->params.manip
                                   : data->params.manip);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_BOOST_SOLINOID:
        LOGI(TAG, "wgc");
        snprintf(buf, sizeof(buf), "WGC: %.2f%%      ",
                 data->params.boost_solenoid);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_IGNITION_TIMING:
        LOGI(TAG, "ign");
        snprintf(buf, sizeof(buf), "IGN: %dBTDC    ",
                 data->params.ignition_timing);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_LOAD:
        LOGI(TAG, "load");
        snprintf(buf, sizeof(buf), "LOAD: %d       ",
                 data->params.engine_load);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_INJECTOR_PW:
        LOGI(TAG, "inj");
        snprintf(buf, sizeof(buf), "IPW: %.3fms   ",
                 data->params.injector_pw);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_IAC:
        LOGI(TAG, "iac");
        snprintf(buf, sizeof(buf), "IAC: %.1f        ",
                 data->params.iac);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_O2_V:
        LOGI(TAG, "o2");
        snprintf(buf, sizeof(buf), "O2: %.2fv     ",
                 data->params.o2_signal);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_TIMING_CORRECTION:
        LOGI(TAG, "timing");
        snprintf(buf, sizeof(buf), "CORR: %d       ",
                 data->params.timing_correction);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_FUEL_TRIM:
        LOGI(TAG, "trim");
        snprintf(buf, sizeof(buf), "TRIM: %.2f      ",
                 data->params.fuel_trim);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_BAROP:
        LOGI(TAG, "barop");
        snprintf(buf, sizeof(buf), "BARO: %.2ftorr   ",
                 data->params.barop);
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        break;

    case STATE_INPUT_SWITCHES:
        LOGI(TAG, "input switch");
        snprintf(buf, sizeof(buf), "IG%d AT%d TM%d RM%d ",
                 data->in_sw.ignition, 
                 data->in_sw.auto_trans,
                 data->in_sw.test_mode,
                 data->in_sw.read_mode
        );
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        snprintf(buf, sizeof(buf), "NT%d PK%d CA%d     ",
                 data->in_sw.neutral,
                 data->in_sw.park,
                 data->in_sw.california
        );
        hd44780_set_cursor(0, 1);
        hd44780_print(buf);
        break;

    case STATE_INOUT_SWITCHES:
        LOGI(TAG, "io switch");
        snprintf(buf, sizeof(buf), "ID%d AC%d AR%d RF%d ",
                 data->io_sw.idle_sw,
                 data->io_sw.ac_sw,
                 data->io_sw.ac_relay,
                 data->io_sw.rad_fan
        );
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        snprintf(buf, sizeof(buf), "FP%d CN%d KS%d PX%d ",
                 data->io_sw.fuel_pump,
                 data->io_sw.purge_valve,
                 data->io_sw.pinging,
                 data->io_sw.press_exch
        );
        hd44780_set_cursor(0, 1);
        hd44780_print(buf);
        break;

    case STATE_STORED_CODE_ONE:
    case STATE_ACTIVE_CODE_ONE:
        LOGI(TAG, "code 1");
        snprintf(buf, sizeof(buf), "11%d 12%d 13%d 14%d ",
                 data->tc1.crank,
                 data->tc1.starter,
                 data->tc1.cam,
                 data->tc1.inj_1
        );
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        snprintf(buf, sizeof(buf), "15%d 16%d 17%d     ",
                 data->tc1.inj_2,
                 data->tc1.inj_3,
                 data->tc1.inj_4
        );
        hd44780_set_cursor(0, 1);
        hd44780_print(buf);
        break;

    case STATE_STORED_CODE_TWO:
    case STATE_ACTIVE_CODE_TWO:
        LOGI(TAG, "code 2");
        snprintf(buf, sizeof(buf), "21%d 22%d 23%d 24%d ",
                 data->tc2.temp,
                 data->tc2.knock,
                 data->tc2.maf,
                 data->tc2.iacv
        );
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        snprintf(buf, sizeof(buf), "31%d 32%d 33%d 35%d ",
                 data->tc2.tps,
                 data->tc2.oxygen,
                 data->tc2.vss,
                 data->tc2.purge
        );
        hd44780_set_cursor(0, 1);
        hd44780_print(buf);
        break;

    case STATE_STORED_CODE_THREE:
    case STATE_ACTIVE_CODE_THREE:
        LOGI(TAG, "code 3");
        snprintf(buf, sizeof(buf), "41%d 42%d 44%d 45%d ",
                 data->tc3.fuel_trim,
                 data->tc3.idle_sw,
                 data->tc3.wgc,
                 data->tc3.baro
        );
        hd44780_set_cursor(0, 0);
        hd44780_print(buf);
        snprintf(buf, sizeof(buf), "49%d 51%d 52%d     ",
                 data->tc3.wrong_maf,
                 data->tc3.neutral_sw,
                 data->tc3.parking_sw
        );
        hd44780_set_cursor(0, 1);
        hd44780_print(buf);
        break;
    }
}