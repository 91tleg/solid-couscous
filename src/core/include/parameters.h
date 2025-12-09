#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

struct __attribute__((packed)) ecu_params 
{
    uint8_t romid[3];
    uint8_t engine_load;

    uint8_t throttle_percentage;
    uint8_t ignition_timing;
    uint8_t vehicle_speed;
    uint8_t timing_correction;

    uint16_t engine_speed;
    int16_t coolant_temp;
    
    float injector_pw;
    float battery_voltage;
    float airflow;
    float iac;
    float boost_solenoid;
    float barop;
    float throttle_signal;
    float manip;
    float o2_signal;
    float fuel_trim;
};

/**
 * Bit fields are inherently non-portable across compilers, and as such guarantee that your code is not reusable.
 * Use #defines and bit masks. This is a highly portable method, and is the one that should be used.
 */

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

#endif // PARAMETERS_H