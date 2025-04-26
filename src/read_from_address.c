#include "read_from_address.h"
#include "ssm1.h"
#include "parameters.h"

uint8_t read_battery_voltage_addr(void)
{
    return read_data_from_address(BATTERY_VOLTAGE_ADDR);
}

uint8_t read_vehicle_speed_addr(void)
{
    return read_data_from_address(VEHICLE_SPEED_ADDR);
}

uint8_t read_engine_speed_addr(void)
{
    return read_data_from_address(ENGINE_SPEED_ADDR);
}

uint8_t read_coolant_temp_addr(void)
{
    return read_data_from_address(COOLANT_TEMP_ADDR);
}

uint8_t read_airflow_addr(void)
{
    return read_data_from_address(AIRFLOW_SENSOR_ADDR);
}

uint8_t read_throttle_addr(void)
{
    return read_data_from_address(THROTTLE_ADDR);
}

uint8_t read_manifold_pressure_addr(void)
{
    return read_data_from_address(MANIFOLD_PRESSURE_ADDR);
}

uint8_t read_boost_solinoid_addr(void)
{
    return read_data_from_address(BOOST_SOLENOID_ADDR);
}

uint8_t read_ignition_timing_addr(void)
{
    return read_data_from_address(IGNITION_ADVANCE_ADDR);
}

uint8_t read_engine_load_addr(void)
{
    return read_data_from_address(ENGINE_LOAD_ADDR);
}

uint8_t read_injector_pulse_width_addr(void)
{
    return read_data_from_address(ENGINE_LOAD_ADDR);
}

uint8_t read_iac_valve_duty_cycle_addr(void)
{
    return read_data_from_address(IAC_VALVE_ADDR);
}

uint8_t read_o2_signal_addr(void)
{
    return read_data_from_address(O2_AVERAGE_ADDR);
}

uint8_t read_timing_correction_addr(void)
{
    return read_data_from_address(TIMING_CORRECTION_ADDR);
}

uint8_t read_fuel_trim_addr(void)
{
    return read_data_from_address(AF_CORRECTION_ADDR);
}

uint8_t read_atmosphere_pressure_addr(void)
{
    return read_data_from_address(ATMOSPHERIC_PRESSURE_ADDR);
}

uint8_t read_input_switches_addr(void)
{
    return read_data_from_address(INPUT_SWITCHES_ADDR);
}

uint8_t read_io_switches_addr(void)
{
    return read_data_from_address(INOUT_SWITCHES_ADDR);
}

uint8_t read_active_trouble_code_one_addr(void)
{
    return read_data_from_address(ACTIVE_TROUBLE_CODE_ONE_ADDR);
}

uint8_t read_active_trouble_code_two_addr(void)
{
    return read_data_from_address(ACTIVE_TROUBLE_CODE_TWO_ADDR);
}

uint8_t read_active_trouble_code_three_addr(void)
{
    return read_data_from_address(ACTIVE_TROUBLE_CODE_THREE_ADDR);
}

uint8_t read_stored_trouble_code_one_addr(void)
{
    return read_data_from_address(STORED_TROUBLE_CODE_ONE_ADDR);
}

uint8_t read_stored_trouble_code_two_addr(void)
{
    return read_data_from_address(STORED_TROUBLE_CODE_TWO_ADDR);
}

uint8_t read_stored_trouble_code_three_addr(void)
{
    return read_data_from_address(STORED_TROUBLE_CODE_THREE_ADDR);
}