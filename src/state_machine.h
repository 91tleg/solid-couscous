#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>
#include <string.h>

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

void state_machine_run(void);

#endif