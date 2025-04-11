#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

#define BATTERY_VOLTAGE_ADDR            (0x1404)
#define VEHICLE_SPEED_ADDR              (0x154B)
#define ENGINE_SPEED_ADDR               (0x140B)
#define COOLANT_TEMP_ADDR               (0x1405)
#define IGNITION_ADVANCE_ADDR           (0x1489)
#define AIRFLOW_SENSOR_ADDR             (0x1400)
#define ENGINE_LOAD_ADDR                (0x1414)
#define THROTTLE_ADDR                   (0x1487)
#define INJECTOR_PULSE_WIDTH_ADDR       (0x15F0)
#define IAC_VALVE_ADDR                  (0x158A)
#define TIMING_CORRECTION_ADDR          (0x1530)
#define O2_AVERAGE_ADDR                 (0x1403)
#define AF_CORRECTION_ADDR              (0x1488)
#define ATMOSPHERIC_PRESSURE_ADDR       (0x1516)
#define MANIFOLD_PRESSURE_ADDR          (0x00BE)
#define BOOST_SOLENOID_ADDR             (0x144D)
#define INPUT_SWITCHES_ADDR             (0x15A8)
#define INOUT_SWITCHES_ADDR             (0x15A9)
#define ACTIVE_TROUBLE_CODE_ONE_ADDR    (0x0047)
#define ACTIVE_TROUBLE_CODE_TWO_ADDR    (0x0048)
#define ACTIVE_TROUBLE_CODE_THREE_ADDR  (0x0049)
#define STORED_TROUBLE_CODE_ONE_ADDR    (0x1604)
#define STORED_TROUBLE_CODE_TWO_ADDR    (0x1605)
#define STORED_TROUBLE_CODE_THREE_ADDR  (0x1606)

struct __attribute__((packed)) ecu_params 
{
    uint8_t romid[3];
    uint8_t load;

    uint8_t throttle_percentage;
    uint8_t ignition_timing;
    uint8_t iac;
    uint8_t timing_correction;

    uint16_t engine_speed;
    int16_t coolant_temp;
    
    float injector_pw;
    float battery_voltage;
    float airflow;
    float vehicle_speed;
    float boost_solenoid;
    float barop;
    float throttle_voltage;
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

#endif