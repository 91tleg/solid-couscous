#include "addr_map.h"
#include "defines.h"
#include "addr.h"

struct state_addr_map
{
    fsm_state_e state;
    uint16_t addr;
};

static const struct state_addr_map addr_map[] = {
    { STATE_BATTERY_V,        BATTERY_VOLTAGE_ADDR },
    { STATE_VEHICLE_SPEED,    VEHICLE_SPEED_ADDR },
    { STATE_ENGINE_SPEED,     ENGINE_SPEED_ADDR },
    { STATE_COOLANT_TEMP,     COOLANT_TEMP_ADDR },
    { STATE_IGNITION_TIMING,  IGNITION_ADVANCE_ADDR },
    { STATE_AIRFLOW,          AIRFLOW_SENSOR_ADDR },
    { STATE_LOAD,             ENGINE_LOAD_ADDR },
    { STATE_THROTTLE,         THROTTLE_ADDR },
    { STATE_INJECTOR_PW,      INJECTOR_PULSE_WIDTH_ADDR },
    { STATE_IAC,              IAC_VALVE_ADDR },
    { STATE_TIMING_CORRECTION,TIMING_CORRECTION_ADDR },
    { STATE_O2_V,             O2_AVERAGE_ADDR },
    { STATE_FUEL_TRIM,        AF_CORRECTION_ADDR },
    { STATE_BAROP,            ATMOSPHERIC_PRESSURE_ADDR },
    { STATE_MANIP,            MANIFOLD_PRESSURE_ADDR },
    { STATE_BOOST_SOLINOID,   BOOST_SOLENOID_ADDR },
    { STATE_INPUT_SWITCHES,   INPUT_SWITCHES_ADDR },
    { STATE_INOUT_SWITCHES,   INOUT_SWITCHES_ADDR },
    { STATE_ACTIVE_CODE_ONE,  ACTIVE_TROUBLE_CODE_ONE_ADDR },
    { STATE_ACTIVE_CODE_TWO,  ACTIVE_TROUBLE_CODE_TWO_ADDR },
    { STATE_ACTIVE_CODE_THREE,ACTIVE_TROUBLE_CODE_THREE_ADDR },
    { STATE_STORED_CODE_ONE,  STORED_TROUBLE_CODE_ONE_ADDR },
    { STATE_STORED_CODE_TWO,  STORED_TROUBLE_CODE_TWO_ADDR },
    { STATE_STORED_CODE_THREE,STORED_TROUBLE_CODE_THREE_ADDR }
};

uint16_t lookup_addr(fsm_state_e s)
{
    for (int i = 0; i < ARRAY_SIZE(addr_map); ++i)
    {
        if (addr_map[i].state == s)
        {
            return addr_map[i].addr;
        }
    }
    return 0;
}