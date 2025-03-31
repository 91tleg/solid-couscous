#include "state_machine.h"
#include "defines.h"
#include "parameters.h"
#include "process_parameters.h"
#include "button.h"

struct state_machine_data
{
    state_e state;
    struct ecu_params parameters;
    struct input_switches status0;
    struct io_switches status1;
    struct trouble_code_one status2;
    struct trouble_code_two status3;
    struct trouble_code_three status4;
};

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
        {STATE_BATTERY_V, STATE_EVENT_BUTTON_PRESS, STATE_VEHICLE_SPEED},
        {STATE_BATTERY_V, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_VEHICLE_SPEED, STATE_EVENT_BUTTON_PRESS, STATE_ENGINE_SPEED},
        {STATE_VEHICLE_SPEED, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_ENGINE_SPEED, STATE_EVENT_BUTTON_PRESS, STATE_COOLANT_TEMP},
        {STATE_ENGINE_SPEED, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_COOLANT_TEMP, STATE_EVENT_BUTTON_PRESS, STATE_AIRFLOW},
        {STATE_COOLANT_TEMP, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_AIRFLOW, STATE_EVENT_BUTTON_PRESS, STATE_THROTTLE},
        {STATE_AIRFLOW, STATE_EVENT_BUTTON_LONG_PRESS, STATE_AIRFLOW},
        {STATE_THROTTLE, STATE_EVENT_BUTTON_PRESS, STATE_THROTTLE_V},
        {STATE_THROTTLE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_THROTTLE_V, STATE_EVENT_BUTTON_PRESS, STATE_MANIP},
        {STATE_THROTTLE_V, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_MANIP, STATE_EVENT_BUTTON_PRESS, STATE_BOOST_SOLINOID},
        {STATE_MANIP, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_BOOST_SOLINOID, STATE_EVENT_BUTTON_PRESS, STATE_IGNITION_TIMING},
        {STATE_BOOST_SOLINOID, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_IGNITION_TIMING, STATE_EVENT_BUTTON_PRESS, STATE_LOAD},
        {STATE_IGNITION_TIMING, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_LOAD, STATE_EVENT_BUTTON_PRESS, STATE_INJECTOR_PW},
        {STATE_LOAD, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_INJECTOR_PW, STATE_EVENT_BUTTON_PRESS, STATE_IAC},
        {STATE_INJECTOR_PW, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_IAC, STATE_EVENT_BUTTON_PRESS, STATE_O2_V},
        {STATE_IAC, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_O2_V, STATE_EVENT_BUTTON_PRESS, STATE_TIMING_CORRECTION},
        {STATE_O2_V, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_TIMING_CORRECTION, STATE_EVENT_BUTTON_PRESS, STATE_FUEL_TRIM},
        {STATE_TIMING_CORRECTION, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_FUEL_TRIM, STATE_EVENT_BUTTON_PRESS, STATE_BAROP},
        {STATE_FUEL_TRIM, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_BAROP, STATE_EVENT_BUTTON_PRESS, STATE_INPUT_SWITCHES},
        {STATE_BAROP, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_INPUT_SWITCHES, STATE_EVENT_BUTTON_PRESS, STATE_INOUT_SWITCHES},
        {STATE_INPUT_SWITCHES, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},

};

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

static void state_enter(struct state_machine_data *data, state_e from, state_event_e event, state_e to)
{
    if (from != to)
    {
        data->state = to;
    }
    switch (to)
    {
    case STATE_ROMID:
        uint8_t romid_buf[3];
        get_romid(romid_buf);
        break;
    case STATE_BATTERY_V:
        data->parameters.battery_voltage = read_battery_voltage();
        break;
    case STATE_VEHICLE_SPEED:
        data->parameters.vehicle_speed = read_vehicle_speed();
        break;
    case STATE_ENGINE_SPEED:
        data->parameters.engine_speed = read_engine_speed();
        break;
    case STATE_COOLANT_TEMP:
        data->parameters.coolant_temp = read_coolant_temp();
        break;
    case STATE_AIRFLOW:
        data->parameters.airflow = read_airflow();
        break;
    case STATE_THROTTLE:
        data->parameters.throttle_percentage = read_throttle_percentage();
        break;
    case STATE_THROTTLE_V:
        data->parameters.throttle_voltage = read_throttle_signal();
        break;
    case STATE_MANIP:
        data->parameters.manip = read_manifold_pressure();
        break;
    case STATE_BOOST_SOLINOID:
        data->parameters.boost_solenoid = read_boost_control_duty_cycle();
        break;
    case STATE_IGNITION_TIMING:
        data->parameters.ignition_timing = read_ignition_timing();
        break;
    case STATE_LOAD:
        data->parameters.load = read_load();
        break;
    case STATE_INJECTOR_PW:
        data->parameters.injector_pw = read_injector_pulse_width();
        break;
    case STATE_IAC:
        data->parameters.iac = read_iacv_duty_cycle();
        break;
    case STATE_O2_V:
        data->parameters.o2_voltage = read_o2_signal();
        break;
    case STATE_TIMING_CORRECTION:
        data->parameters.timing_correction = read_timing_correction();
        break;
    case STATE_FUEL_TRIM:
        data->parameters.fuel_trim = read_fuel_trim();
        break;
    case STATE_BAROP:
        data->parameters.barop = read_atmosphere_pressure();
        break;
    case STATE_INPUT_SWITCHES:
        read_input_switches();
        break;
    case STATE_INOUT_SWITCHES:
        read_inout_switches();
        break;
    case STATE_ACTIVE_CODE_ONE:
        read_active_trouble_code_one();
        break;
    case STATE_ACTIVE_CODE_TWO:
        read_active_trouble_code_two();
        break;
    case STATE_ACTIVE_CODE_THREE:
        read_active_trouble_code_three();
        break;
    case STATE_STORED_CODE_ONE:
        read_stored_trouble_code_one();
        break;
    case STATE_STORED_CODE_TWO:
        read_stored_trouble_code_two();
        break;
    case STATE_STORED_CODE_THREE:
        read_stored_trouble_code_three();
        break;
    }
}

static inline void process_event(struct state_machine_data *data, state_event_e event)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(state_transitions); i++)
    {
        if (data->state == state_transitions[i].from && event == state_transitions[i].event)
        {
            state_enter(data, state_transitions[i].from, event, state_transitions[i].to);
        }
    }
}

static inline state_event_e process_input(struct state_machine_data *data)
{
    return read_state_event();
}

void state_machine_run(void)
{
    struct state_machine_data data;
    state_machine_init(&data);

    for (;;)
    {
        const state_event_e next_event = process_input(&data);
        process_event(&data, next_event);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}