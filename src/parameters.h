#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

struct ecu_params
{
	uint8_t load;
	uint8_t vehicle_speed;
	uint8_t throttle_percentage;
	uint8_t ignition_timing;
	uint8_t iac;
	uint8_t timing_correction;
	int16_t coolant_temp;
	uint16_t engine_speed;
	float boost_solinoid;
	float barop;
	float battery_voltage;
	float airflow;
	float throttle_voltage;
	float injector_pw;
	float manip;
	float o2_voltage;
	float fuel_trim;
};

struct input_switches
{
    uint8_t ignition : 1;
    uint8_t auto_trans : 1;
    uint8_t test_mode : 1;
    uint8_t read_mode : 1;
    uint8_t unused_bit : 1;
    uint8_t neutral : 1;
    uint8_t park : 1;
    uint8_t california : 1;
};

struct io_switches
{
    uint8_t idle_sw : 1;
    uint8_t ac_sw : 1;
    uint8_t ac_relay : 1;
    uint8_t rad_fan : 1;
    uint8_t fuel_pump : 1;
    uint8_t purge_valve : 1;
    uint8_t pinging : 1;
    uint8_t press_exch : 1;
};

struct trouble_code_one
{
    uint8_t crank : 1;
    uint8_t starter : 1;
    uint8_t cam : 1;
    uint8_t inj_1 : 1;
    uint8_t inj_2 : 1;
    uint8_t inj_3 : 1;
    uint8_t inj_4 : 1;
    uint8_t unused_bit_7 : 1;
};

struct trouble_code_two
{
    uint8_t temp : 1;
    uint8_t knock : 1;
    uint8_t maf : 1;
    uint8_t iacv : 1;
    uint8_t tps : 1;
    uint8_t oxygen : 1;
    uint8_t vss : 1;
    uint8_t purge : 1;
};

struct trouble_code_three
{
    uint8_t fuel_trim : 1;
    uint8_t idle_sw : 1;
    uint8_t unused_bit_2 : 1;
    uint8_t wgc : 1;
    uint8_t baro : 1;
    uint8_t wrong_maf : 1;
    uint8_t neutral_sw : 1;
    uint8_t parking_sw : 1;
};

extern const uint8_t coolant_lookup_table[];
extern struct ecu_params parameters;
extern struct input_switches status0;
extern struct io_switches status1;
extern struct trouble_code_one status2;
extern struct trouble_code_two status3;
extern struct trouble_code_three status4;

#endif