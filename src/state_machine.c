#include "state_machine.h"
#include "string.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "defines.h"
#include "button.h"
#include "uart.h"
#include "get_parameters.h"
#include "ssm1.h"
#include "stdbool.h"

QueueHandle_t lcd_queue;

struct state_transition
{
    state_e from;
    state_event_e event; // Event that triggers the transition
    state_e to;
};

static const struct state_transition state_transitions[] __attribute__((section(".rodata"))) =
    {
        {STATE_ROMID, STATE_EVENT_BUTTON_PRESS, STATE_BATTERY_V},
        {STATE_ROMID, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_ROMID, STATE_EVENT_NONE, STATE_ROMID},
        {STATE_BATTERY_V, STATE_EVENT_BUTTON_PRESS, STATE_VEHICLE_SPEED},
        {STATE_BATTERY_V, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_BATTERY_V, STATE_EVENT_NONE, STATE_BATTERY_V},
        {STATE_VEHICLE_SPEED, STATE_EVENT_BUTTON_PRESS, STATE_ENGINE_SPEED},
        {STATE_VEHICLE_SPEED, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_VEHICLE_SPEED, STATE_EVENT_NONE, STATE_VEHICLE_SPEED},
        {STATE_ENGINE_SPEED, STATE_EVENT_BUTTON_PRESS, STATE_COOLANT_TEMP},
        {STATE_ENGINE_SPEED, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_ENGINE_SPEED, STATE_EVENT_NONE, STATE_ENGINE_SPEED},
        {STATE_COOLANT_TEMP, STATE_EVENT_BUTTON_PRESS, STATE_AIRFLOW},
        {STATE_COOLANT_TEMP, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_COOLANT_TEMP, STATE_EVENT_NONE, STATE_COOLANT_TEMP},
        {STATE_AIRFLOW, STATE_EVENT_BUTTON_PRESS, STATE_THROTTLE},
        {STATE_AIRFLOW, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_AIRFLOW, STATE_EVENT_NONE, STATE_AIRFLOW},
        {STATE_THROTTLE, STATE_EVENT_BUTTON_PRESS, STATE_THROTTLE_V},
        {STATE_THROTTLE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_THROTTLE, STATE_EVENT_NONE, STATE_THROTTLE},
        {STATE_THROTTLE_V, STATE_EVENT_BUTTON_PRESS, STATE_MANIP},
        {STATE_THROTTLE_V, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_THROTTLE_V, STATE_EVENT_NONE, STATE_THROTTLE_V},
        {STATE_MANIP, STATE_EVENT_BUTTON_PRESS, STATE_BOOST_SOLINOID},
        {STATE_MANIP, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_MANIP, STATE_EVENT_NONE, STATE_MANIP},
        {STATE_BOOST_SOLINOID, STATE_EVENT_BUTTON_PRESS, STATE_IGNITION_TIMING},
        {STATE_BOOST_SOLINOID, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_BOOST_SOLINOID, STATE_EVENT_NONE, STATE_BOOST_SOLINOID},
        {STATE_IGNITION_TIMING, STATE_EVENT_BUTTON_PRESS, STATE_LOAD},
        {STATE_IGNITION_TIMING, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_IGNITION_TIMING, STATE_EVENT_NONE, STATE_IGNITION_TIMING},
        {STATE_LOAD, STATE_EVENT_BUTTON_PRESS, STATE_INJECTOR_PW},
        {STATE_LOAD, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_LOAD, STATE_EVENT_NONE, STATE_LOAD},
        {STATE_INJECTOR_PW, STATE_EVENT_BUTTON_PRESS, STATE_IAC},
        {STATE_INJECTOR_PW, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_INJECTOR_PW, STATE_EVENT_NONE, STATE_INJECTOR_PW},
        {STATE_IAC, STATE_EVENT_BUTTON_PRESS, STATE_O2_V},
        {STATE_IAC, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_IAC, STATE_EVENT_NONE, STATE_IAC},
        {STATE_O2_V, STATE_EVENT_BUTTON_PRESS, STATE_TIMING_CORRECTION},
        {STATE_O2_V, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_O2_V, STATE_EVENT_NONE, STATE_O2_V},
        {STATE_TIMING_CORRECTION, STATE_EVENT_BUTTON_PRESS, STATE_FUEL_TRIM},
        {STATE_TIMING_CORRECTION, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_TIMING_CORRECTION, STATE_EVENT_NONE, STATE_TIMING_CORRECTION},
        {STATE_FUEL_TRIM, STATE_EVENT_BUTTON_PRESS, STATE_BAROP},
        {STATE_FUEL_TRIM, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_FUEL_TRIM, STATE_EVENT_NONE, STATE_FUEL_TRIM},
        {STATE_BAROP, STATE_EVENT_BUTTON_PRESS, STATE_INPUT_SWITCHES},
        {STATE_BAROP, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_BAROP, STATE_EVENT_NONE, STATE_BAROP},
        {STATE_INPUT_SWITCHES, STATE_EVENT_BUTTON_PRESS, STATE_INOUT_SWITCHES},
        {STATE_INPUT_SWITCHES, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_INPUT_SWITCHES, STATE_EVENT_NONE, STATE_INPUT_SWITCHES},
        {STATE_INOUT_SWITCHES, STATE_EVENT_BUTTON_PRESS, STATE_ACTIVE_CODE_ONE},
        {STATE_INOUT_SWITCHES, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_INOUT_SWITCHES, STATE_EVENT_NONE, STATE_INOUT_SWITCHES},
        {STATE_ACTIVE_CODE_ONE, STATE_EVENT_BUTTON_PRESS, STATE_ACTIVE_CODE_TWO},
        {STATE_ACTIVE_CODE_ONE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_ACTIVE_CODE_ONE, STATE_EVENT_NONE, STATE_ACTIVE_CODE_ONE},
        {STATE_ACTIVE_CODE_TWO, STATE_EVENT_BUTTON_PRESS, STATE_ACTIVE_CODE_THREE},
        {STATE_ACTIVE_CODE_TWO, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_ACTIVE_CODE_TWO, STATE_EVENT_NONE, STATE_ACTIVE_CODE_TWO},
        {STATE_ACTIVE_CODE_THREE, STATE_EVENT_BUTTON_PRESS, STATE_STORED_CODE_ONE},
        {STATE_ACTIVE_CODE_THREE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_ACTIVE_CODE_THREE, STATE_EVENT_NONE, STATE_ACTIVE_CODE_THREE},
        {STATE_STORED_CODE_ONE, STATE_EVENT_BUTTON_PRESS, STATE_STORED_CODE_TWO},
        {STATE_STORED_CODE_ONE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_STORED_CODE_ONE, STATE_EVENT_NONE, STATE_STORED_CODE_ONE},
        {STATE_STORED_CODE_TWO, STATE_EVENT_BUTTON_PRESS, STATE_STORED_CODE_THREE},
        {STATE_STORED_CODE_TWO, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_STORED_CODE_TWO, STATE_EVENT_NONE, STATE_STORED_CODE_TWO},
        {STATE_STORED_CODE_THREE, STATE_EVENT_BUTTON_PRESS, STATE_ROMID},
        {STATE_STORED_CODE_THREE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_STORED_CODE_THREE, STATE_EVENT_NONE, STATE_STORED_CODE_THREE}};

static inline void state_machine_init(struct state_machine_data *data)
{
    data->state = STATE_ROMID;
    memset(&data->parameters, 0, sizeof(data->parameters));
    memset(&data->status0, 0, sizeof(data->status0));
    memset(&data->status1, 0, sizeof(data->status1));
    memset(&data->status2, 0, sizeof(data->status2));
    memset(&data->status3, 0, sizeof(data->status3));
    memset(&data->status4, 0, sizeof(data->status4));
}

static void state_enter(struct state_machine_data *data, state_e from, state_e to)
{
    if (from != to)
    {
        data->state = to;
    }
    switch (data->state)
    {
    case STATE_ROMID:
        ESP_LOGI("SM", "ROMID");
        if (data->parameters.romid[0] == 0x00)
        {
            bool success = get_romid(data->parameters.romid);
            if (!success)
            {
                data->parameters.romid[0] = 0x00;
                data->parameters.romid[1] = 0x00;
                data->parameters.romid[2] = 0x00;
            }
        }
        break;
    case STATE_BATTERY_V:
        data->parameters.battery_voltage = get_battery_voltage();
        ESP_LOGI("SM", "BATV: %f", data->parameters.battery_voltage);
        break;
    case STATE_VEHICLE_SPEED:
        data->parameters.vehicle_speed = get_vehicle_speed();
        ESP_LOGI("SM", "SPD: %f", data->parameters.vehicle_speed);
        break;
    case STATE_ENGINE_SPEED:
        data->parameters.engine_speed = get_engine_speed();
        ESP_LOGI("SM", "RPM: %u", data->parameters.engine_speed);
        break;
    case STATE_COOLANT_TEMP:
        data->parameters.coolant_temp = get_coolant_temp();
        ESP_LOGI("SM", "COOLANT: %d", data->parameters.coolant_temp);
        break;
    case STATE_AIRFLOW:
        data->parameters.airflow = get_airflow();
        ESP_LOGI("SM", "MAF: %f", data->parameters.airflow);
        break;
    case STATE_THROTTLE:
        data->parameters.throttle_percentage = get_throttle_percentage();
        ESP_LOGI("SM", "THROTTLE: %u", data->parameters.throttle_percentage);
        break;
    case STATE_THROTTLE_V:
        // TODO
        data->parameters.throttle_signal = get_throttle_signal();
        ESP_LOGI("SM", "THROTTLEV: %f", data->parameters.throttle_signal);
        break;
    case STATE_MANIP:
        // TODO
        data->parameters.manip = get_manifold_pressure();
        ESP_LOGI("SM", "MANIP: %f", data->parameters.manip);
        break;
    case STATE_BOOST_SOLINOID:
        data->parameters.boost_solenoid = get_boost_control_duty_cycle();
        ESP_LOGI("SM", "WGC: %f", data->parameters.boost_solenoid);
        break;
    case STATE_IGNITION_TIMING:
        data->parameters.ignition_timing = get_ignition_timing();
        ESP_LOGI("SM", "IGN: %u", data->parameters.ignition_timing);
        break;
    case STATE_LOAD:
        data->parameters.engine_load = get_engine_load();
        ESP_LOGI("SM", "LOAD: %u", data->parameters.engine_load);
        break;
    case STATE_INJECTOR_PW:
        data->parameters.injector_pw = get_injector_pulse_width();
        ESP_LOGI("SM", "INJ: %f", data->parameters.injector_pw);
        break;
    case STATE_IAC:
        data->parameters.iac = get_iacv_duty_cycle();
        ESP_LOGI("SM", "IAC: %u", data->parameters.iac);
        break;
    case STATE_O2_V:
        data->parameters.o2_signal = get_o2_signal();
        ESP_LOGI("SM", "O2V: %f", data->parameters.o2_signal);
        break;
    case STATE_TIMING_CORRECTION:
        data->parameters.timing_correction = get_timing_correction();
        ESP_LOGI("SM", "TIM: %u", data->parameters.timing_correction);
        break;
    case STATE_FUEL_TRIM:
        data->parameters.fuel_trim = get_fuel_trim();
        ESP_LOGI("SM", "TRIM: %f", data->parameters.fuel_trim);
        break;
    case STATE_BAROP:
        data->parameters.barop = get_atmosphere_pressure();
        ESP_LOGI("SM", "BAROP: %f", data->parameters.barop);
        break;
    case STATE_INPUT_SWITCHES:
        data->status0 = get_input_switches();
        ESP_LOGI("SM", "IN");
        break;
    case STATE_INOUT_SWITCHES:
        data->status1 = get_io_switches();
        ESP_LOGI("SM", "IO");
        break;
    case STATE_ACTIVE_CODE_ONE:
        data->status2 = get_active_trouble_code_one();
        ESP_LOGI("SM", "A1");
        break;
    case STATE_ACTIVE_CODE_TWO:
        data->status3 = get_active_trouble_code_two();
        ESP_LOGI("SM", "A2");
        break;
    case STATE_ACTIVE_CODE_THREE:
        data->status4 = get_active_trouble_code_three();
        ESP_LOGI("SM", "A3");
        break;
    case STATE_STORED_CODE_ONE:
        data->status2 = get_stored_trouble_code_one();
        ESP_LOGI("SM", "S1");
        break;
    case STATE_STORED_CODE_TWO:
        data->status3 = get_stored_trouble_code_two();
        ESP_LOGI("SM", "S2");
        break;
    case STATE_STORED_CODE_THREE:
        data->status4 = get_stored_trouble_code_three();
        ESP_LOGI("SM", "S3");
        break;
    }
    xQueueSend(lcd_queue, (void *)data, portMAX_DELAY);
}

static inline void process_event(struct state_machine_data *data, state_event_e event)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(state_transitions); i++)
    {
        if ((data->state == state_transitions[i].from) && (event == state_transitions[i].event))
        {
            state_enter(data, state_transitions[i].from, state_transitions[i].to);
            break;
        }
    }
}

static inline state_event_e process_input(void)
{
    state_event_e recieved_event;
    if (xQueueReceive(event_queue, &recieved_event, pdMS_TO_TICKS(50)) == pdPASS)
    {
        return recieved_event;
    }
    return STATE_EVENT_NONE;
}

void state_machine_task(void *parameters)
{
    static struct state_machine_data data;
    uart_init();
    ESP_LOGI("SM", "uart initialized");
    state_machine_init(&data);
    ecu_init();
    for (;;)
    {
        state_event_e next_event = process_input();
        process_event(&data, next_event);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}