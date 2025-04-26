#ifndef GET_PARAMETERS_H
#define GET_PARAMETERS_H 

#include <stdint.h>

float get_battery_voltage(void);
float get_vehicle_speed(void);
uint16_t get_engine_speed(void);
int16_t get_coolant_temp(void);
float get_airflow(void);
uint8_t get_throttle_percentage(void);
float get_throttle_signal(void);
float get_manifold_pressure(void);
float get_boost_control_duty_cycle(void);
uint8_t get_ignition_timing(void);
uint8_t get_engine_load(void);
float get_injector_pulse_width(void);
float get_iacv_duty_cycle(void);
float get_o2_signal(void);
uint8_t get_timing_correction(void);
float get_fuel_trim(void);
float get_atmosphere_pressure(void);
struct input_switches get_input_switches(void);
struct io_switches get_io_switches(void);
struct trouble_code_one get_active_trouble_code_one(void);
struct trouble_code_two get_active_trouble_code_two(void);
struct trouble_code_three get_active_trouble_code_three(void);
struct trouble_code_one get_stored_trouble_code_one(void);
struct trouble_code_two get_stored_trouble_code_two(void);
struct trouble_code_three get_stored_trouble_code_three(void);

#endif // GET_PARAMETERS_H