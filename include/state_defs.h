#ifndef STATE_DEFS_H
#define STATE_DEFS_H

#include "parameters.h"

typedef enum
{
    STATE_EVENT_BUTTON_PRESS,
    STATE_EVENT_BUTTON_LONG_PRESS,
    STATE_EVENT_BUTTON_DOUBLE_PRESS,
    STATE_EVENT_NONE
} state_event_e;

typedef enum
{
    STATE_ROMID,
    STATE_BATTERY_V,
    STATE_VEHICLE_SPEED,
    STATE_ENGINE_SPEED,
    STATE_COOLANT_TEMP,
    STATE_AIRFLOW,
    STATE_THROTTLE,
    STATE_THROTTLE_V,
    STATE_MANIP,
    STATE_BOOST_SOLINOID,
    STATE_IGNITION_TIMING,
    STATE_LOAD,
    STATE_INJECTOR_PW,
    STATE_IAC,
    STATE_O2_V,
    STATE_TIMING_CORRECTION,
    STATE_FUEL_TRIM,
    STATE_BAROP,
    STATE_INPUT_SWITCHES,
    STATE_INOUT_SWITCHES,
    STATE_ACTIVE_CODE_ONE,
    STATE_ACTIVE_CODE_TWO,
    STATE_ACTIVE_CODE_THREE,
    STATE_STORED_CODE_ONE,
    STATE_STORED_CODE_TWO,
    STATE_STORED_CODE_THREE
} state_e;

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

#endif // STATE_DEFS_H