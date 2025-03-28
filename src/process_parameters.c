#include "process_parameters.h"

float read_battery_voltage(void)
{
    // uint8_t value = read_data_from_address(BATTERY_VOLTAGE_ADDR);
    float result = 0.0;

    __asm__ (
        "nop"
    );
    return result;
}
uint8_t read_speed(void)
{
    uint8_t value = read_data_from_address(SPEED_ADDR);
    uint8_t result;

    __asm__ volatile (
        "l8ui a2, %1, 0\n"   // Load value into a2
        "movi a3, 10\n"      // Load 10 into a3
        "mull a2, a2, a3\n"  // Multiply a2 by a3
        "movi a3, 16\n"      // Load 16 into a3
        "quou a2, a2, a3\n"  // Divide a2 by a3
        "mov %0, a2\n"       // Store result in output variable
        : "=r" (result)      // Output
        : "r" (&value)       // Input
        : "a2", "a3"         // Clobbered registers
    );
    return result;
}

uint16_t read_rpm(void)
{
    uint8_t value = read_data_from_address(RPM_ADDR);
    uint16_t result;

    __asm__ volatile (
        "l16ui a2, %1, 0\n"
        "movi a3, 25\n"
        "mull a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r" (result)
        : "r" (value)
        : "a2", "a3"
    );
    return result;
}

int16_t read_coolant_temp(void)
{
    uint8_t index = read_data_from_address(COOLANT_ADDR);
    int16_t value = (int16_t)(coolant_lookup_table[index]);

    __asm__ volatile (
        "l8ui %0, %1, 0\n"        // Load value from lookup table
        : "=r" (value) 
        : "r" (&coolant_lookup_table[index])
        : "memory"
    );

    __asm__ volatile (
        "cmp %0, #14\n"           // Compare index with 14
        "blt .add_255\n"          // If index < 14 branch to add_255
        "b .check_negative\n"     // Else skip the add_255 part
        ".add_255:\n"
        "add %1, %1, #255\n"      // Add 255 to value
        "b .done_check\n"         // Skip the next part
        ".check_negative:\n"
        "cmp %0, #227\n"          // Compare index with 227
        "bge .negate_value\n"     // If index >= 227, branch to negate_value
        "b .done_check\n"         // Skip negate_value part
        ".negate_value:\n"
        "rsb %1, %1, #0\n"        // Reverse subtract value from 0
        ".done_check:\n"
        : "=r" (index), "=r" (value)
        : "0" (index), "1" (value)
        : "memory", "cc"
    );
    return value;
    //if (index < 14) value += 255;
    //else if (index >= 256 - 29) value = -value;
    //ecu_parameters.tw = value;
}

void read_airflow(void)
{
    ecu_parameters.qa = (float)(read_data_from_address(AIRFLOW_SENSOR_ADDR)) / 50;
}

void read_throttle_percentage(void)
{
    ecu_parameters.tps = read_data_from_address(THROTTLE_ADDR) * 100 / 256;
}

void read_throttle_signal(void)
{
    ecu_parameters.thv = (float)(read_data_from_address(THROTTLE_ADDR)) * 100 / 256;
}

void read_manifold_pressure(void)
{
    ecu_parameters.manip = (float)(read_data_from_address(MANIFOLD_PRESSURE_ADDR)) / 0.128 - 1060;
}

void read_boost_control_duty_cycle(void)
{
    ecu_parameters.wgc = (float)(read_data_from_address(BOOST_SOLENOID_ADDR) * 100) / 256;
}
void read_ignition_timing(void)
{
    ecu_parameters.advs = read_data_from_address(IGNITION_ADVANCE_ADDR);
}

void read_load(void)
{
    ecu_parameters.ldata = read_data_from_address(ENGINE_LOAD_ADDR);
}

void read_injector_pulse_width(void)
{
    ecu_parameters.tim = (float)(read_data_from_address(INJECTOR_PULSE_WIDTH_ADDR)) * 2.048;
}

uint8_t read_iacv_duty_cycle(void)
{
    uint8_t value = read_data_from_address(ISU_DUTY_VALVE_ADDR);
    uint8_t result;

    __asm__ volatile (
        "l8ui a2, %1, 0\n"
        "movi a3, 2\n"
        "quou a2, a2, a3\n"
        "mov %0, a2;"
        : "=r" (result)
        : "r" (value)
        : "a2", "a3"
    );
    return result;
}

void read_o2_signal(void)
{
    ecu_parameters.o2r = (float)(read_data_from_address(O2_AVERAGE_ADDR)) / 100;
}

void read_timing_correction(void)
{
    ecu_parameters.rtrd = read_data_from_address(TIMING_CORRECTION_ADDR);
}

void read_fuel_trim(void)
{
    ecu_parameters.alphar = (float)(read_data_from_address(AF_CORRECTION_ADDR) - 128) / 1.28;
}

void read_atmosphere_pressure(void)
{
    ecu_parameters.barop = (float)(930 - read_data_from_address(ATMOSPHERIC_PRESSURE_ADDR)) * 3.09;
}

void read_input_switches(void)
{
    uint8_t value = read_data_from_address(INPUT_SWITCHES_ADDR);
    status.ignition = (value & (1 << 7)) ? 1 : 0;
    status.auto_trans = (value & (1 << 6)) ? 1 : 0;
    status.test_mode = (value & (1 << 5)) ? 1 : 0;
    status.read_mode = (value & (1 << 4)) ? 1 : 0;
    status.neutral = (value & (1 << 2)) ? 1 : 0;
    status.park = (value & (1 << 1)) ? 1 : 0;
    status.california = (value & (1 << 0)) ? 1 : 0;
}

void read_io_switches(void)
{
    uint8_t value = read_data_from_address(IO_SWITCHES_ADDR);
    status0.idle_sw = (value & (1 << 7)) ? 1 : 0;
    status0.ac_sw = (value & (1 << 6)) ? 1 : 0;
    status0.ac_relay = (value & (1 << 5)) ? 1 : 0;
    status0.rad_fan = (value & (1 << 4)) ? 1 : 0;
    status0.fuel_pump = (value & (1 << 3)) ? 1 : 0;
    status0.purge_valve = (value & (1 << 2)) ? 1 : 0;
    status0.pinging = (value & (1 << 1)) ? 1 : 0;
    status0.press_exch = (value & (1 << 0)) ? 1 : 0;
}

void read_trouble_code_one(uint16_t addr)
{
    uint8_t value = read_data_from_address(addr);
    status1.crank = (value & (1 << 7)) ? 1 : 0;
    status1.starter = (value & (1 << 6)) ? 1 : 0;
    status1.cam = (value & (1 << 5)) ? 1 : 0;
    status1.inj_1 = (value & (1 << 4)) ? 1 : 0;
    status1.inj_2 = (value & (1 << 3)) ? 1 : 0;
    status1.inj_3 = (value & (1 << 2)) ? 1 : 0;
    status1.inj_4 = (value & (1 << 1)) ? 1 : 0;
}

void read_active_trouble_code_one(void)
{
    read_trouble_code_one(ACTIVE_TROUBLE_CODE_ONE_ADDR);
}

void read_stored_trouble_code_one(void)
{
    read_trouble_code_one(STORED_TROUBLE_CODE_THREE_ADDR);
}

void read_trouble_code_two(uint16_t addr)
{
    uint8_t value = read_data_from_address(addr);
    status2.temp = (value & (1 << 7)) ? 1 : 0;
    status2.knock = (value & (1 << 6)) ? 1 : 0;
    status2.maf = (value & (1 << 5)) ? 1 : 0;
    status2.iacv = (value & (1 << 4)) ? 1 : 0;
    status2.tps = (value & (1 << 3)) ? 1 : 0;
    status2.oxygen = (value & (1 << 2)) ? 1 : 0;
    status2.vss = (value & (1 << 1)) ? 1 : 0;
    status2.purge = (value & (1 << 0)) ? 1 : 0;
}

void read_active_trouble_code_two(void)
{
    read_trouble_code_two(ACTIVE_TROUBLE_CODE_TWO_ADDR);
}

void read_stored_trouble_code_two(void)
{
    read_trouble_code_two(STORED_TROUBLE_CODE_TWO_ADDR);
}

void read_trouble_code_three(uint16_t addr)
{
    uint8_t value = read_data_from_address(addr);
    status3.fuel_trim = (value & (1 << 7)) ? 1 : 0;
    status3.idle_sw = (value & (1 << 6)) ? 1 : 0;
    status3.wgc = (value & (1 << 4)) ? 1 : 0;
    status3.baro = (value & (1 << 3)) ? 1 : 0;
    status3.wrong_maf = (value & (1 << 2)) ? 1 : 0;
    status3.neutral_sw = (value & (1 << 1)) ? 1 : 0;
    status3.parking_sw = (value & (1 << 0)) ? 1 : 0;
}

void read_active_trouble_code_three(void)
{
    read_trouble_code_three(ACTIVE_TROUBLE_CODE_THREE_ADDR);
}

void read_stored_trouble_code_three(void)
{
    read_trouble_code_three(STORED_TROUBLE_CODE_THREE_ADDR);
}