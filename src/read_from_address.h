#ifndef READ_FROM_ADDRESS_H
#define READ_FROM_ADDRESS_H 

#include <stdint.h>

uint8_t read_battery_voltage_addr(void);
uint8_t read_vehicle_speed_addr(void);
uint8_t read_engine_speed_addr(void);
uint8_t read_coolant_temp_addr(void);
uint8_t read_airflow_addr(void);
uint8_t read_throttle_addr(void);
uint8_t read_manifold_pressure_addr(void);
uint8_t read_boost_solinoid_addr(void);
uint8_t read_ignition_timing_addr(void);
uint8_t read_engine_load_addr(void);
uint8_t read_injector_pulse_width_addr(void);
uint8_t read_iac_valve_duty_cycle_addr(void);
uint8_t read_o2_signal_addr(void);
uint8_t read_timing_correction_addr(void);
uint8_t read_fuel_trim_addr(void);
uint8_t read_atmosphere_pressure_addr(void);
uint8_t read_input_switches_addr(void);
uint8_t read_io_switches_addr(void);
uint8_t read_active_trouble_code_one_addr(void);
uint8_t read_active_trouble_code_two_addr(void);
uint8_t read_active_trouble_code_three_addr(void);
uint8_t read_active_trouble_code_three_addr(void);
uint8_t read_stored_trouble_code_one_addr(void);
uint8_t read_stored_trouble_code_two_addr(void);
uint8_t read_stored_trouble_code_three_addr(void);

#endif // READ_FROM_ADDRESS_H