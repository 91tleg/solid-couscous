#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

#define BATTERY_VOLTAGE_ADDR            (0X1404U)
#define VEHICLE_SPEED_ADDR              (0X154BU)
#define ENGINE_SPEED_ADDR               (0X140BU)
#define COOLANT_TEMP_ADDR               (0X1405U)
#define IGNITION_ADVANCE_ADDR           (0X1489U)
#define AIRFLOW_SENSOR_ADDR             (0X1400U)
#define ENGINE_LOAD_ADDR                (0X1414U)
#define THROTTLE_ADDR                   (0X1487U)
#define INJECTOR_PULSE_WIDTH_ADDR       (0X15F0U)
#define IAC_VALVE_ADDR                  (0X158AU)
#define TIMING_CORRECTION_ADDR          (0X1530U)
#define O2_AVERAGE_ADDR                 (0X1403U)
#define AF_CORRECTION_ADDR              (0X1488U)
#define ATMOSPHERIC_PRESSURE_ADDR       (0X1516U)
#define MANIFOLD_PRESSURE_ADDR          (0X00BEU)
#define BOOST_SOLENOID_ADDR             (0X144DU)
#define INPUT_SWITCHES_ADDR             (0X15A8U)
#define INOUT_SWITCHES_ADDR             (0X15A9U)
#define ACTIVE_TROUBLE_CODE_ONE_ADDR    (0X0047U)
#define ACTIVE_TROUBLE_CODE_TWO_ADDR    (0X0048U)
#define ACTIVE_TROUBLE_CODE_THREE_ADDR  (0X0049U)
#define STORED_TROUBLE_CODE_ONE_ADDR    (0X1604U)
#define STORED_TROUBLE_CODE_TWO_ADDR    (0X1605U)
#define STORED_TROUBLE_CODE_THREE_ADDR  (0X1606U)

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

union param_type
{
    uint8_t u8;
    int8_t i8;
    uint16_t u16;
    int16_t i16;
    float f32;
    struct input_switches in_sw;
    struct io_switches io_sw;
    struct trouble_code_one tc_one;
    struct trouble_code_two tc_two;
    struct trouble_code_three tc_three;
}

#endif // PARAMETERS_H