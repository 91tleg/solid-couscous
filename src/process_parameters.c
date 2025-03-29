#include "process_parameters.h"

uint16_t read_battery_voltage(void)
{
    uint8_t value = read_data_from_address(BATTERY_VOLTAGE_ADDR);
    uint16_t result;

    __asm__ volatile(
        "mov a2, %1\n"         // Move value into a2 (no need to load)
        "movi a3, 8\n"         // Load 8 into a3
        "extui a2, a2, 0, 8\n" // Extend a2 to 16 bits
        "mull a2, a2, a3\n"    // Multiply a2 by a3 (a2 * 8)
        "mov %0, a2\n"         // Store result in output
        : "=r"(result)         // Output
        : "r"(value)           // Input
        : "a2", "a3");         // Clobbered registers
    return result;             // Need to /100
}

uint8_t read_speed(void)
{
    uint8_t value = read_data_from_address(SPEED_ADDR);
    uint8_t result;

    __asm__ volatile(
        "mov a2, %1\n"         // Load value into a2
        "extui a2, a2, 0, 8\n" // Extend a2 to 16 bits
        "movi a3, 10\n"        // Load 10 into a3
        "mull a2, a2, a3\n"    // Multiply a2 by a3
        "movi a3, 16\n"        // Load 16 into a3
        "quou a2, a2, a3\n"    // Divide a2 by a3
        "mov %0, a2\n"         // Store result in output
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
}

uint16_t read_rpm(void)
{
    uint8_t value = read_data_from_address(RPM_ADDR);
    uint16_t result;

    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 25\n"
        "extui a2, a2, 0, 8\n" // Extend a2 to 16 bits
        "mull a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
}

int16_t read_coolant_temp(void)
{
    uint8_t index = read_data_from_address(COOLANT_ADDR);
    int16_t result;

    __asm__ volatile(
        "mov a2, %1\n"      // Load value from lookup table
        "movi a3, 14\n"         // Load 14 into a3
        "sub a3, a2, a3\n"      // a3 = a2 - 14
        "movi a4, 0\n"          // Load 0 into a4
        "bge a3, a4, add_255\n" // If a3 >= 0 (a2 >= 14), jump to add_255
        "j done_check\n"        // Skip adding 255 if condition fails

        "add_255:\n"
        "addi a2, a2, 255\n" // Add 255 to a2

        "done_check:\n"
        "movi a3, 227\n"               // Load 227 into a3
        "sub a3, a2, a3\n"             // a3 = a2 - 227
        "blt a3, a4, done_check_end\n" // If a3 < 0 (a2 < 227), skip negation
        "neg a2, a2\n"                 // Negate a2

        "done_check_end:\n"
        : "=r"(result)
        : "r"(&coolant_lookup_table[index])
        : "a2", "a3", "a4", "memory");
    return result;
}

void read_airflow(void)
{
    // ecu_parameters.qa = (float)(read_data_from_address(AIRFLOW_SENSOR_ADDR)) / 50;
}

uint8_t read_throttle_percentage(void)
{
    uint8_t value = read_data_from_address(THROTTLE_ADDR);
    uint8_t result;

    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 100\n"
        "mull a2, a2, a3\n"
        "movi a3, 256\n"
        "quou a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
    // ecu_parameters.tps = read_data_from_address(THROTTLE_ADDR) * 100 / 256;
}

void read_throttle_signal(void)
{
    // ecu_parameters.thv = (float)(read_data_from_address(THROTTLE_ADDR)) * 100 / 256;
}

void read_manifold_pressure(void)
{
    // ecu_parameters.manip = (float)(read_data_from_address(MANIFOLD_PRESSURE_ADDR)) / 0.128 - 1060;
}

void read_boost_control_duty_cycle(void)
{
    // ecu_parameters.wgc = (float)(read_data_from_address(BOOST_SOLENOID_ADDR) * 100) / 256;
}
void read_ignition_timing(void)
{
    // ecu_parameters.advs = read_data_from_address(IGNITION_ADVANCE_ADDR);
}

void read_load(void)
{
    // ecu_parameters.ldata = read_data_from_address(ENGINE_LOAD_ADDR);
}

void read_injector_pulse_width(void)
{
    // ecu_parameters.tim = (float)(read_data_from_address(INJECTOR_PULSE_WIDTH_ADDR)) * 2.048;
}

uint8_t read_iacv_duty_cycle(void)
{
    uint8_t value = read_data_from_address(ISU_DUTY_VALVE_ADDR);
    uint8_t result;

    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 2\n"
        "quou a2, a2, a3\n"
        "mov %0, a2;"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
}

uint8_t read_o2_signal(void)
{
    uint8_t value = read_data_from_address(O2_AVERAGE_ADDR);
    return value; // Need /100
    // ecu_parameters.o2r = (float)(read_data_from_address(O2_AVERAGE_ADDR)) / 100;
}

uint8_t read_timing_correction(void)
{
    uint8_t value = read_data_from_address(TIMING_CORRECTION_ADDR);
    return value;
    // ecu_parameters.rtrd = read_data_from_address(TIMING_CORRECTION_ADDR);
}

void read_fuel_trim(void)
{
    // ecu_parameters.alphar = (float)(read_data_from_address(AF_CORRECTION_ADDR) - 128) / 1.28;
}

void read_atmosphere_pressure(void)
{
    // ecu_parameters.barop = (float)(930 - read_data_from_address(ATMOSPHERIC_PRESSURE_ADDR)) * 3.09;
}

void read_input_switches(void)
{
    uint8_t value = read_data_from_address(INPUT_SWITCHES_ADDR);

    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"

        // Ignition (bit 7)
        "slli a3, a2, 7\n" // Shift a2 right by 7
        "and a3, a3, a4\n" // Isolate the bit
        "mov %1, a3\n"     // Store result in status.ignition

        // Auto Transmission (bit 6)
        "slli a3, a2, 6\n" // Shift a2 right by 6
        "and a3, a3, a4\n"
        "mov %2, a3\n" // Store result in status.auto_trans

        // Test Mode (bit 5)
        "slli a3, a2, 5\n" // Shift a2 right by 5
        "and a3, a3, a4\n"
        "mov %3, a3\n" // Store result in status.test_mode

        // Read Mode (bit 4)
        "slli a3, a2, 4\n" // Shift a2 right by 4
        "and a3, a3, a4\n"
        "mov %4, a3\n" // Store result in status.read_mode

        // Neutral (bit 2)
        "slli a3, a2, 2\n" // Shift a2 right by 2
        "and a3, a3, a4\n"
        "mov %5, a3\n" // Store result in status.neutral

        // Park (bit 1)
        "slli a3, a2, 1\n" // Shift a2 right by 1
        "and a3, a3, a4\n"
        "mov %6, a3\n" // Store result in status.park

        // California (bit 0)
        "slli a3, a2, 0\n" // Shift a2 right by 0
        "and a3, a3, a4\n"
        "mov %7, a3\n" // Store result in status.california

        : "=r"(status0.ignition), "=r"(status0.auto_trans), "=r"(status0.test_mode),
          "=r"(status0.read_mode), "=r"(status0.neutral), "=r"(status0.park), "=r"(status0.california)
        : "r"(value)
        : "a2", "a3", "a4", "memory");
}

void read_io_switches(void)
{
    /*
    uint8_t value = read_data_from_address(IO_SWITCHES_ADDR);
    status0.idle_sw = (value & (1 << 7)) ? 1 : 0;
    status0.ac_sw = (value & (1 << 6)) ? 1 : 0;
    status0.ac_relay = (value & (1 << 5)) ? 1 : 0;
    status0.rad_fan = (value & (1 << 4)) ? 1 : 0;
    status0.fuel_pump = (value & (1 << 3)) ? 1 : 0;
    status0.purge_valve = (value & (1 << 2)) ? 1 : 0;
    status0.pinging = (value & (1 << 1)) ? 1 : 0;
    status0.press_exch = (value & (1 << 0)) ? 1 : 0;
}*/}

    void read_trouble_code_one(uint16_t addr)
    {
        uint8_t value = read_data_from_address(addr);
        status2.crank = (value & (1 << 7)) ? 1 : 0;
        status2.starter = (value & (1 << 6)) ? 1 : 0;
        status2.cam = (value & (1 << 5)) ? 1 : 0;
        status2.inj_1 = (value & (1 << 4)) ? 1 : 0;
        status2.inj_2 = (value & (1 << 3)) ? 1 : 0;
        status2.inj_3 = (value & (1 << 2)) ? 1 : 0;
        status2.inj_4 = (value & (1 << 1)) ? 1 : 0;
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
        status3.temp = (value & (1 << 7)) ? 1 : 0;
        status3.knock = (value & (1 << 6)) ? 1 : 0;
        status3.maf = (value & (1 << 5)) ? 1 : 0;
        status3.iacv = (value & (1 << 4)) ? 1 : 0;
        status3.tps = (value & (1 << 3)) ? 1 : 0;
        status3.oxygen = (value & (1 << 2)) ? 1 : 0;
        status3.vss = (value & (1 << 1)) ? 1 : 0;
        status3.purge = (value & (1 << 0)) ? 1 : 0;
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
        status4.fuel_trim = (value & (1 << 7)) ? 1 : 0;
        status4.idle_sw = (value & (1 << 6)) ? 1 : 0;
        status4.wgc = (value & (1 << 4)) ? 1 : 0;
        status4.baro = (value & (1 << 3)) ? 1 : 0;
        status4.wrong_maf = (value & (1 << 2)) ? 1 : 0;
        status4.neutral_sw = (value & (1 << 1)) ? 1 : 0;
        status4.parking_sw = (value & (1 << 0)) ? 1 : 0;
    }

    void read_active_trouble_code_three(void)
    {
        read_trouble_code_three(ACTIVE_TROUBLE_CODE_THREE_ADDR);
    }

    void read_stored_trouble_code_three(void)
    {
        read_trouble_code_three(STORED_TROUBLE_CODE_THREE_ADDR);
    }