#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

typedef struct
{
	uint8_t ldata;
	uint8_t vsp;
	uint8_t tps;
	uint8_t advs;
	uint8_t isc;
	uint8_t rtrd;
	int16_t tw;
	uint16_t erev;
	float wgc;
	float barop;
	float vb;
	float qa;
	float thv;
	float tim;
	float manip;
	float o2r;
	float alphar;
} ecu_params;

typedef struct
{
    uint8_t ignition : 1;
    uint8_t auto_trans : 1;
    uint8_t test_mode : 1;
    uint8_t read_mode : 1;
    uint8_t unused_bit : 1;
    uint8_t neutral : 1;
    uint8_t park : 1;
    uint8_t california : 1;
} input_switches;

typedef struct
{
    uint8_t idle_sw : 1;
    uint8_t ac_sw : 1;
    uint8_t ac_relay : 1;
    uint8_t rad_fan : 1;
    uint8_t fuel_pump : 1;
    uint8_t purge_valve : 1;
    uint8_t pinging : 1;
    uint8_t press_exch : 1;
} io_switches;

typedef struct
{
    uint8_t crank : 1;
    uint8_t starter : 1;
    uint8_t cam : 1;
    uint8_t inj_1 : 1;
    uint8_t inj_2 : 1;
    uint8_t inj_3 : 1;
    uint8_t inj_4 : 1;
    uint8_t unused_bit_7 : 1;
} trouble_code_one;

typedef struct
{
    uint8_t temp : 1;
    uint8_t knock : 1;
    uint8_t maf : 1;
    uint8_t iacv : 1;
    uint8_t tps : 1;
    uint8_t oxygen : 1;
    uint8_t vss : 1;
    uint8_t purge : 1;
} trouble_code_two;

typedef struct
{
    uint8_t fuel_trim : 1;
    uint8_t idle_sw : 1;
    uint8_t unused_bit_2 : 1;
    uint8_t wgc : 1;
    uint8_t baro : 1;
    uint8_t wrong_maf : 1;
    uint8_t neutral_sw : 1;
    uint8_t parking_sw : 1;
} trouble_code_three;

extern const uint8_t coolant_lookup_table[];
extern ecu_params ecu_parameters;
extern input_switches status;
extern io_switches status0;
extern trouble_code_one status1;
extern trouble_code_two status2;
extern trouble_code_three status3;

#endif