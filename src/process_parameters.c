#include "process_parameters.h"

float read_battery_voltage(void)
{
    uint8_t value = read_data_from_address(BATTERY_VOLTAGE_ADDR);
    float result;

    __asm__ volatile(
        "mov a2, %1\n"         // Move value into a2
        "movi a3, 8\n"         // Move 8 into a3
        "extui a2, a2, 0, 8\n" // Extend a2 to 16 bits
        "mull a2, a2, a3\n"    // Multiply a2 by a3
        "ufloat.s f0, a2, 1\n"
        "movi a2, 100\n"
        "ufloat.s f1, a2, 1\n"
        "divn.s f0, f0, f1\n"
        "mov.s %0, f0\n"         // Store result in output
        : "=f"(result)         // Output
        : "r"(value)           // Input
        : "a2", "a3", "f0", "f1");         // Clobbered registers
    return result;             // Need to /100
}

uint8_t read_vehicle_speed(void)
{
    uint8_t value = read_data_from_address(SPEED_ADDR);
    uint8_t result;

    __asm__ volatile(
        "mov a2, %1\n"
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

uint16_t read_engine_speed(void)
{
    uint8_t value = read_data_from_address(RPM_ADDR);
    uint16_t result;

    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 25\n"
        "extui a2, a2, 0, 8\n"
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
        "mov a2, %1\n"          // Load value from lookup table
        "extui a2, a2, 0, 8\n"
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

uint16_t read_airflow(void)
{
    uint8_t value = read_data_from_address(AIRFLOW_SENSOR_ADDR);
    uint16_t result;
    __asm__ volatile(
        "mov a2, %1\n"
        "extui a2, a2, 0, 8\n"
        "movi a3, 2\n"
        "mull a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3"
    );
    return result; // Need /100
}

uint8_t read_throttle_percentage(void)
{
    uint8_t value = read_data_from_address(THROTTLE_ADDR);
    uint8_t result;

    __asm__ volatile(
        "mov a2, %1\n"
        "extui a2, a2, 0, 8\n"
        "movi a3, 100\n"
        "mull a2, a2, a3\n"
        "movi a3, 256\n"
        "quou a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
}

uint16_t read_throttle_signal(void)
{
    uint8_t value = read_data_from_address(BOOST_SOLENOID_ADDR);
    uint16_t result = 0;

    return result;
    // ecu_parameters.thv = (float)(read_data_from_address(THROTTLE_ADDR)) * 100 / 256;
}

int16_t read_manifold_pressure(void)
{
    uint8_t value = read_data_from_address(BOOST_SOLENOID_ADDR);
    int16_t result = 0;

    return result;
    // ecu_parameters.manip = (float)(read_data_from_address(MANIFOLD_PRESSURE_ADDR)) / 0.128 - 1060;
}

uint16_t read_boost_control_duty_cycle(void)
{
    uint8_t value = read_data_from_address(BOOST_SOLENOID_ADDR);
    uint16_t result;
    __asm__ volatile(
        "mov a2, %1\n"
        "extui a2, a2, 0, 8\n"
        "movi a3, 39\n"
        "mull a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3"
    );
    return result;  // Need /100
}

uint8_t read_ignition_timing(void)
{
    uint8_t value =  read_data_from_address(IGNITION_ADVANCE_ADDR);
    return value;
}

uint8_t read_load(void)
{
    uint8_t value = read_data_from_address(ENGINE_LOAD_ADDR);
    return value;
}

uint16_t read_injector_pulse_width(void)
{
    uint8_t value = read_data_from_address(INJECTOR_PULSE_WIDTH_ADDR);
    uint16_t result;

    __asm__ volatile(
        "mov a2, %1\n"
        "extui a2, a2, 0, 8\n"
        "movi a3, 2048\n"
        "mull a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;  // Need /1000
}

uint8_t read_iacv_duty_cycle(void)
{
    uint8_t value = read_data_from_address(ISU_DUTY_VALVE_ADDR);
    uint8_t result;

    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 2\n"
        "quou a2, a2, a3\n"
        "mov %0, a2\n;"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
}

uint8_t read_o2_signal(void)
{
    uint8_t value = read_data_from_address(O2_AVERAGE_ADDR);
    return value; // Need /100
}

uint8_t read_timing_correction(void)
{
    uint8_t value = read_data_from_address(TIMING_CORRECTION_ADDR);
    return value;
}

int16_t read_fuel_trim(void)
{
    uint8_t value = read_data_from_address(TIMING_CORRECTION_ADDR);
    int16_t result;

    __asm__ volatile(
        "mov a2, %1\n"
        "extui a2, a2, 0, 8\n"
        "movi a3, 128\n"
        "sub a2, a2, a3\n"
        "movi a3, 78\n"
        "mull a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result; // Need /100
}

float read_atmosphere_pressure(void)
{
    uint8_t value = read_data_from_address(ATMOSPHERIC_PRESSURE_ADDR);
    float result;

    __asm__ volatile(
        "mov a2, %1\n"
        "ufloat.s f0, a2, 1\n"     // Convert a2 to float and store in f0
        "movi a2, 930\n"
        "ufloat.s f1, a2, 1\n" 
        "sub.s f0, f1, f0\n"       // f0 = f1 - f0
        "movi a2, 309\n"           // Move 309 into a2
        "ufloat.s f2, a2, 1\n"     // Convert integer 309 to float in f2
        "mul.s f0, f0, f2\n"       // f0 = f0 * f2 (f0 * 3.09)
        "mov.s %0, f0\n"
        : "=f"(result)
        : "r"(value)
        : "a2", "f0", "f1", "f2"
    );
    return result;
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

void read_inout_switches(void)
{
    uint8_t value = read_data_from_address(INOUT_SWITCHES_ADDR);

    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"

        // Idle switch (bit 7)
        "slli a3, a2, 7\n" // Shift a2 right by 7
        "and a3, a3, a4\n" // Isolate the bit
        "mov %1, a3\n"

        // AC switch (bit 6)
        "slli a3, a2, 6\n" // Shift a2 right by 6
        "and a3, a3, a4\n"
        "mov %2, a3\n"

        // AC relay (bit 5)
        "slli a3, a2, 5\n" // Shift a2 right by 5
        "and a3, a3, a4\n"
        "mov %3, a3\n"

        // Rad fan (bit 4)
        "slli a3, a2, 4\n" // Shift a2 right by 4
        "and a3, a3, a4\n"
        "mov %4, a3\n"

        // Fuel pump (bit 3)
        "slli a3, a2, 3\n" // Shift a2 right by 2
        "and a3, a3, a4\n"
        "mov %5, a3\n"

        // Purge valve (bit 2)
        "slli a3, a2, 2\n" // Shift a2 right by 2
        "and a3, a3, a4\n"
        "mov %6, a3\n"

        // Pinging (bit 1)
        "slli a3, a2, 1\n" // Shift a2 right by 1
        "and a3, a3, a4\n"
        "mov %7, a3\n"

        // Pressure exchange (bit 0)
        "slli a3, a2, 0\n" // Shift a2 right by 0
        "and a3, a3, a4\n"
        "mov %8, a3\n"

        : "=r"(status1.idle_sw), "=r"(status1.ac_sw), "=r"(status1.ac_relay), "=r"(status1.rad_fan), 
        "=r"(status1.fuel_pump), "=r"(status1.purge_valve), "=r"(status1.pinging), "=r"(status1.press_exch)
        : "r"(value)
        : "a2", "a3", "a4", "memory");
}

void read_trouble_code_one(uint16_t addr)
{
    uint8_t value = read_data_from_address(addr);

    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"

        // Crank (bit 7)
        "slli a3, a2, 7\n" // Shift a2 right by 7
        "and a3, a3, a4\n" // Isolate the bit
        "mov %1, a3\n"

        // Starter (bit 6)
        "slli a3, a2, 6\n" // Shift a2 right by 6
        "and a3, a3, a4\n"
        "mov %2, a3\n"

        // Cam (bit 5)
        "slli a3, a2, 5\n" // Shift a2 right by 5
        "and a3, a3, a4\n"
        "mov %3, a3\n"

        // Injector one (bit 4)
        "slli a3, a2, 4\n" // Shift a2 right by 4
        "and a3, a3, a4\n"
        "mov %4, a3\n"

        // Injector two (bit 3)
        "slli a3, a2, 3\n" // Shift a2 right by 2
        "and a3, a3, a4\n"
        "mov %5, a3\n"

        // Injector three (bit 2)
        "slli a3, a2, 2\n" // Shift a2 right by 2
        "and a3, a3, a4\n"
        "mov %6, a3\n"

        // Injector four (bit 1)
        "slli a3, a2, 1\n" // Shift a2 right by 1
        "and a3, a3, a4\n"
        "mov %7, a3\n"

        : "=r"(status2.crank), "=r"(status2.starter), "=r"(status2.cam), "=r"(status2.inj_1), 
        "=r"(status2.inj_2), "=r"(status2.inj_3), "=r"(status2.inj_4)
        : "r"(value)
        : "a2", "a3", "a4", "memory");
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

    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"

        // Temp (bit 7)
        "slli a3, a2, 7\n" // Shift a2 right by 7
        "and a3, a3, a4\n" // Isolate the bit
        "mov %1, a3\n"

        // Knock (bit 6)
        "slli a3, a2, 6\n" // Shift a2 right by 6
        "and a3, a3, a4\n"
        "mov %2, a3\n"

        // MAF (bit 5)
        "slli a3, a2, 5\n" // Shift a2 right by 5
        "and a3, a3, a4\n"
        "mov %3, a3\n"

        // IAC (bit 4)
        "slli a3, a2, 4\n" // Shift a2 right by 4
        "and a3, a3, a4\n"
        "mov %4, a3\n"

        // TPS (bit 3)
        "slli a3, a2, 3\n" // Shift a2 right by 2
        "and a3, a3, a4\n"
        "mov %5, a3\n"

        // O2 (bit 2)
        "slli a3, a2, 2\n" // Shift a2 right by 2
        "and a3, a3, a4\n"
        "mov %6, a3\n"

        // VSS (bit 1)
        "slli a3, a2, 1\n" // Shift a2 right by 1
        "and a3, a3, a4\n"
        "mov %7, a3\n"

        // Purge (bit 0)
        "slli a3, a2, 0\n" // Shift a2 right by 0
        "and a3, a3, a4\n"
        "mov %8, a3\n"

        : "=r"(status3.temp), "=r"(status3.knock), "=r"(status3.maf), "=r"(status3.iacv), 
        "=r"(status3.tps), "=r"(status3.oxygen), "=r"(status3.vss), "=r"(status3.purge)
        : "r"(value)
        : "a2", "a3", "a4", "memory");
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

    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"

        // Fuel trim (bit 7)
        "slli a3, a2, 7\n" // Shift a2 right by 7
        "and a3, a3, a4\n" // Isolate the bit
        "mov %1, a3\n"

        // Idle switch (bit 6)
        "slli a3, a2, 6\n" // Shift a2 right by 6
        "and a3, a3, a4\n"
        "mov %2, a3\n"

        // WGC (bit 4)
        "slli a3, a2, 4\n" // Shift a2 right by 4
        "and a3, a3, a4\n"
        "mov %3, a3\n"

        // Baro (bit 3)
        "slli a3, a2, 3\n" // Shift a2 right by 2
        "and a3, a3, a4\n"
        "mov %4, a3\n"

        // Wrong MAF (bit 2)
        "slli a3, a2, 2\n" // Shift a2 right by 2
        "and a3, a3, a4\n"
        "mov %5, a3\n"

        // Neutral switch (bit 1)
        "slli a3, a2, 1\n" // Shift a2 right by 1
        "and a3, a3, a4\n"
        "mov %6, a3\n"

        // Parking switch (bit 0)
        "slli a3, a2, 0\n" // Shift a2 right by 0
        "and a3, a3, a4\n"
        "mov %7, a3\n"

        : "=r"(status4.fuel_trim), "=r"(status4.idle_sw), "=r"(status4.wgc), "=r"(status4.baro), 
        "=r"(status4.wrong_maf), "=r"(status4.neutral_sw), "=r"(status4.parking_sw)
        : "r"(value)
        : "a2", "a3", "a4", "memory");
}

void read_active_trouble_code_three(void)
{
    read_trouble_code_three(ACTIVE_TROUBLE_CODE_THREE_ADDR);
}

void read_stored_trouble_code_three(void)
{
    read_trouble_code_three(STORED_TROUBLE_CODE_THREE_ADDR);
}
