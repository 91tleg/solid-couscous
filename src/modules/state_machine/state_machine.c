#include "state_machine.h"
#include "string.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "defines.h"
#include "button.h"
#include "get_parameters.h"
#include "ssm1.h"

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

static void state_machine_init(struct state_machine_data *data)
{
    memset(data, 0, sizeof(*data));
    data->state = STATE_ROMID;
}

static void state_enter(struct state_machine_data *data, state_e from, state_e to)
{
    if (from != to)
    {
        data->state = to;
        reset_ctx_sent_flag(from);
        //uart_flush_input(UART_NUM);
    }
    switch (data->state)
    {
    case STATE_ROMID:
        ESP_LOGI("SM", "ROMID");
        get_romid(data->parameters.romid);
        break;
    case STATE_BATTERY_V:
        data->parameters.battery_voltage = get_param(STATE_BATTERY_V).f;
        ESP_LOGI("SM", "BATV: %f", data->parameters.battery_voltage);
        break;
    case STATE_VEHICLE_SPEED:
        data->parameters.vehicle_speed = get_param(STATE_VEHICLE_SPEED).u8;
        ESP_LOGI("SM", "SPD: %u", data->parameters.vehicle_speed);
        break;
    case STATE_ENGINE_SPEED:
        data->parameters.engine_speed = get_param(STATE_ENGINE_SPEED).u16;
        ESP_LOGI("SM", "RPM: %u", data->parameters.engine_speed);
        break;
    case STATE_COOLANT_TEMP:
        data->parameters.coolant_temp = get_param(STATE_COOLANT_TEMP).i16;
        ESP_LOGI("SM", "COOLANT: %d", data->parameters.coolant_temp);
        break;
    case STATE_AIRFLOW:
        data->parameters.airflow = get_param(STATE_AIRFLOW).f;
        ESP_LOGI("SM", "MAF: %f", data->parameters.airflow);
        break;
    case STATE_THROTTLE:
        data->parameters.throttle_percentage = get_param(STATE_THROTTLE).u8;
        ESP_LOGI("SM", "THROTTLE: %u", data->parameters.throttle_percentage);
        break;
    case STATE_THROTTLE_V:
        data->parameters.throttle_signal = get_param(STATE_THROTTLE_V).f;
        ESP_LOGI("SM", "THROTTLEV: %f", data->parameters.throttle_signal);
        break;
    case STATE_MANIP:
        data->parameters.manip = get_param(STATE_MANIP).f;
        ESP_LOGI("SM", "MANIP: %f", data->parameters.manip);
        break;
    case STATE_BOOST_SOLINOID:
        data->parameters.boost_solenoid = get_param(STATE_BOOST_SOLINOID).f;
        ESP_LOGI("SM", "WGC: %f", data->parameters.boost_solenoid);
        break;
    case STATE_IGNITION_TIMING:
        data->parameters.ignition_timing = get_param(STATE_IGNITION_TIMING).u8;
        ESP_LOGI("SM", "IGN: %u", data->parameters.ignition_timing);
        break;
    case STATE_LOAD:
        data->parameters.engine_load = get_param(STATE_LOAD).u8;
        ESP_LOGI("SM", "LOAD: %u", data->parameters.engine_load);
        break;
    case STATE_INJECTOR_PW:
        data->parameters.injector_pw = get_param(STATE_INJECTOR_PW).f;
        ESP_LOGI("SM", "INJ: %f", data->parameters.injector_pw);
        break;
    case STATE_IAC:
        data->parameters.iac = get_param(STATE_IAC).f;
        ESP_LOGI("SM", "IAC: %f", data->parameters.iac);
        break;
    case STATE_O2_V:
        data->parameters.o2_signal = get_param(STATE_O2_V).f;
        ESP_LOGI("SM", "O2V: %f", data->parameters.o2_signal);
        break;
    case STATE_TIMING_CORRECTION:
        data->parameters.timing_correction = get_param(STATE_TIMING_CORRECTION).u8;
        ESP_LOGI("SM", "TIM: %u", data->parameters.timing_correction);
        break;
    case STATE_FUEL_TRIM:
        data->parameters.fuel_trim = get_param(STATE_FUEL_TRIM).f;
        ESP_LOGI("SM", "TRIM: %f", data->parameters.fuel_trim);
        break;
    case STATE_BAROP:
        data->parameters.barop = get_param(STATE_BAROP).f;
        ESP_LOGI("SM", "BAROP: %f", data->parameters.barop);
        break;
    case STATE_INPUT_SWITCHES:
        data->status0 = get_param(STATE_INPUT_SWITCHES).in_sw;
        ESP_LOGI("SM", "IN");
        break;
    case STATE_INOUT_SWITCHES:
        data->status1 = get_param(STATE_INOUT_SWITCHES).io_sw;
        ESP_LOGI("SM", "IO");
        break;
    case STATE_ACTIVE_CODE_ONE:
        data->status2 = get_param(STATE_ACTIVE_CODE_ONE).tc_one;
        ESP_LOGI("SM", "A1");
        break;
    case STATE_ACTIVE_CODE_TWO:
        data->status3 = get_param(STATE_ACTIVE_CODE_TWO).tc_two;
        ESP_LOGI("SM", "A2");
        break;
    case STATE_ACTIVE_CODE_THREE:
        data->status4 = get_param(STATE_ACTIVE_CODE_THREE).tc_three;
        ESP_LOGI("SM", "A3");
        break;
    case STATE_STORED_CODE_ONE:
        data->status2 = get_param(STATE_STORED_CODE_ONE).tc_one;
        ESP_LOGI("SM", "S1");
        break;
    case STATE_STORED_CODE_TWO:
        data->status3 = get_param(STATE_STORED_CODE_TWO).tc_two;
        ESP_LOGI("SM", "S2");
        break;
    case STATE_STORED_CODE_THREE:
        data->status4 = get_param(STATE_STORED_CODE_THREE).tc_three;
        ESP_LOGI("SM", "S3");
        break;
    }

    struct state_machine_data copy = *data;
    xQueueSend(lcd_queue, &copy, portMAX_DELAY);
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
    state_event_e received_event;
    if (xQueueReceive(event_queue, &received_event, pdMS_TO_TICKS(0)) == pdPASS)
    {
        return received_event;
    }
    return STATE_EVENT_NONE;
}

void state_machine_task(void *parameters)
{
    static struct state_machine_data data;
    state_machine_init(&data);

    for (;;)
    {
        state_event_e next_event = process_input();
        process_event(&data, next_event);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}