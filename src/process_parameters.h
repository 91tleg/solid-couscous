#ifndef PROCESS_PARAMETERS_H  
#define PROCESS_PARAMETERS_H  

#include <stdint.h>

float decode_battery_voltage(uint8_t value);
uint8_t decode_vehicle_speed(uint8_t value);
uint16_t decode_engine_speed(uint8_t value);
int16_t decode_coolant_temp(uint8_t value);
float decode_airflow(uint8_t value);
uint8_t decode_throttle_percentage(uint8_t value);
float decode_throttle_signal(uint8_t value);
float decode_manifold_pressure(uint8_t value);
uint8_t decode_boost_control_duty_cycle(uint8_t value);
uint8_t decode_ignition_timing(uint8_t value);
uint8_t decode_engine_load(uint8_t value);
float decode_injector_pulse_width(uint8_t value);
float decode_iacv_duty_cycle(uint8_t value);
float decode_o2_signal(uint8_t value);
uint8_t decode_timing_correction(uint8_t value);
float decode_fuel_trim(uint8_t value);
float decode_atmosphere_pressure(uint8_t value);

#endif // PROCESS_PARAMETERS_H 