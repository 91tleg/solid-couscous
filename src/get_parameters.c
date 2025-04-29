#include "get_parameters.h"
#include "parameters.h"
#include "read_from_address.h"
#include "process_parameters.h"
#include "decode_trouble_codes.h"

float get_battery_voltage(void)
{
    return decode_battery_voltage(read_battery_voltage_addr());
}

float get_vehicle_speed(void)
{
    return decode_vehicle_speed(read_vehicle_speed_addr());
}

uint16_t get_engine_speed(void)
{
    return decode_engine_speed(read_engine_speed_addr());
}

int16_t get_coolant_temp(void)
{
    return decode_coolant_temp(read_coolant_temp_addr());
}

float get_airflow(void)
{
    return decode_airflow(read_airflow_addr());
}

uint8_t get_throttle_percentage(void)
{
    return decode_throttle_percentage(read_throttle_addr());
}

float get_throttle_signal(void)
{
    return decode_throttle_signal(read_throttle_addr());
}

float get_manifold_pressure(void)
{
    return decode_manifold_pressure(read_manifold_pressure_addr());
}

float get_boost_control_duty_cycle(void)
{
    return decode_boost_control_duty_cycle(read_boost_solinoid_addr());
}

uint8_t get_ignition_timing(void)
{
    return decode_ignition_timing(read_ignition_timing_addr());
}

uint8_t get_engine_load(void)
{
    return decode_engine_load(read_engine_load_addr());
}

float get_injector_pulse_width(void)
{
    return decode_injector_pulse_width(read_injector_pulse_width_addr());
}

float get_iacv_duty_cycle(void)
{
    return decode_iacv_duty_cycle(read_iac_valve_duty_cycle_addr());
}

float get_o2_signal(void)
{
    return decode_o2_signal(read_o2_signal_addr());
}

uint8_t get_timing_correction(void)
{
    return decode_timing_correction(read_timing_correction_addr());
}

float get_fuel_trim(void)
{
    return decode_fuel_trim(read_fuel_trim_addr());
}

float get_atmosphere_pressure(void)
{
    return decode_atmosphere_pressure(read_atmosphere_pressure_addr());
}

struct input_switches get_input_switches(void)
{
    struct input_switches dtc;
    decode_input_switches(read_input_switches_addr(), &dtc);
    return dtc;
}

struct io_switches get_io_switches(void)
{
    struct io_switches dtc;
    decode_io_switches(read_io_switches_addr(), &dtc);
    return dtc;
}

struct trouble_code_one get_active_trouble_code_one(void)
{
    struct trouble_code_one dtc;
    decode_trouble_code_one(read_active_trouble_code_one_addr(), &dtc);
    return dtc;
}

struct trouble_code_two get_active_trouble_code_two(void)
{
    struct trouble_code_two dtc;
    decode_trouble_code_two(read_active_trouble_code_two_addr(), &dtc);
    return dtc;
}

struct trouble_code_three get_active_trouble_code_three(void)
{
    struct trouble_code_three dtc;
    decode_trouble_code_three(read_active_trouble_code_three_addr(), &dtc);
    return dtc;
}

struct trouble_code_one get_stored_trouble_code_one(void)
{
    struct trouble_code_one dtc;
    decode_trouble_code_one(read_stored_trouble_code_one_addr(), &dtc);
    return dtc;
}

struct trouble_code_two get_stored_trouble_code_two(void)
{
    struct trouble_code_two dtc;
    decode_trouble_code_two(read_stored_trouble_code_two_addr(), &dtc);
    return dtc;
}

struct trouble_code_three get_stored_trouble_code_three(void)
{
    struct trouble_code_three dtc;
    decode_trouble_code_three(read_stored_trouble_code_three_addr(), &dtc);
    return dtc;
}