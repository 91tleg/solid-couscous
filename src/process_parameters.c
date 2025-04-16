#include "process_parameters.h"
#include "parameters.h"
#include "defines.h"

__attribute__((always_inline)) static inline float recipsf2(float a)
{
    float result;
    __asm__ volatile(
        "wfr f1, %1\n" // Write from register

        "recip0.s f0, f1\n"    // Store the recip in f0
        "const.s f2, 1\n"      // Load 1 into f1
        "msub.s f2, f1, f0\n"  // f2 = f2 - (f1 * f0)
        "maddn.s f0, f0, f2\n" // f0 = f0 + (f0 * f2)
        "const.s f2, 1\n"      // Load 1 into f1 again
        "msub.s f2, f1, f0\n"  // f2 = f2 - (f1 * f0)
        "maddn.s f0, f0, f2\n" // f0 = f0 + (f0 * f2)

        "rfr %0, f0\n" // Read from register
        : "=r"(result)
        : "r"(a)
        : "f0", "f1", "f2");
    return result;
}

void read_battery_voltage(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(BATTERY_VOLTAGE_ADDR);
    float recip = recipsf2(100.0);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 8\n"
        "mull a2, a2, a3\n"
        "ufloat.s f0, a2, 1\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(data->parameters.battery_voltage)
        : "r"(value), "r"(recip)
        : "a2", "a3", "f0", "f1");
}

void read_vehicle_speed(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(VEHICLE_SPEED_ADDR);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 10\n"
        "mull a2, a2, a3\n"
        "srli a2, a2, 4\n" // Divide by 16
        "mov %0, a2\n"
        : "=r"(data->parameters.vehicle_speed)
        : "r"(value)
        : "a2", "a3");
}

void read_engine_speed(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(ENGINE_SPEED_ADDR);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 25\n"
        "mull a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r"(data->parameters.engine_speed)
        : "r"(value)
        : "a2", "a3");
}

void read_coolant_temp(struct state_machine_data *data)
{
    uint8_t index = read_data_from_address(COOLANT_TEMP_ADDR);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 14\n"
        "sub a3, a2, a3\n" // a3 = a2 - 14
        "movi a4, 0\n"
        "bge a3, a4, add_255\n" // If a3 >= 0 (a2 >= 14), jump to add_255
        "j done_check\n"        // Skip adding 255 if condition fails

        "add_255:\n"
        "addi a2, a2, 255\n" // Add 255 to a2

        "done_check:\n"
        "movi a3, 227\n"
        "sub a3, a2, a3\n"             // a3 = a2 - 227
        "blt a3, a4, done_check_end\n" // If a3 < 0 (a2 < 227), skip negation
        "neg a2, a2\n"                 // Negate a2

        "done_check_end:\n"
        : "=r"(data->parameters.coolant_temp)
        : "r"(&coolant_lookup_table[index])
        : "a2", "a3", "a4", "memory");
}

void read_airflow(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(AIRFLOW_SENSOR_ADDR);
    float recip = recipsf2(50.0);
    __asm__ volatile(
        "mov a2, %1\n"
        "ufloat.s f0, a2, 1\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(data->parameters.battery_voltage)
        : "r"(value), "r"(recip)
        : "a2", "f0", "f1");
}

void read_throttle_percentage(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(THROTTLE_ADDR);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 100\n"
        "mull a2, a2, a3\n"
        "srli a2, a2, 8\n" // Devide by 256
        "mov %0, a2\n"
        : "=r"(data->parameters.throttle_percentage)
        : "r"(value)
        : "a2", "a3");
}

void read_throttle_signal(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(BOOST_SOLENOID_ADDR);

    //TODO:
    UNUSED(value);
    // ecu_parameters.thv = (float)(read_data_from_address(THROTTLE_ADDR)) * 100 / 256;
}

void read_manifold_pressure(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(BOOST_SOLENOID_ADDR);

    // TODO:
    UNUSED(value);
    // ecu_parameters.manip = (float)(read_data_from_address(MANIFOLD_PRESSURE_ADDR)) / 0.128 - 1060;
}

void read_boost_control_duty_cycle(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(BOOST_SOLENOID_ADDR);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 100\n"
        "mull a2, a2, a3\n"
        "srli a2, a2, 8\n" // Devide by 256
        "mov %0, a2\n"
        : "=r"(data->parameters.boost_solenoid)
        : "r"(value)
        : "a2", "a3");
}

void read_ignition_timing(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(IGNITION_ADVANCE_ADDR);
    data->parameters.ignition_timing = value; 
}

void read_load(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(ENGINE_LOAD_ADDR);
    data->parameters.load = value;
}

void read_injector_pulse_width(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(INJECTOR_PULSE_WIDTH_ADDR);
    float recip = recipsf2(7.8);
    __asm__ volatile(
        "mov a2, %1\n"
        "ufloat.s f0, a2, 1\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(data->parameters.injector_pw)
        : "r"(value), "r"(recip)
        : "a2", "f0", "f1");
}

void read_iacv_duty_cycle(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(IAC_VALVE_ADDR);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 2\n"
        "quou a2, a2, a3\n"
        "mov %0, a2\n;"
        : "=r"(data->parameters.iac)
        : "r"(value)
        : "a2", "a3");
}

void read_o2_signal(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(O2_AVERAGE_ADDR);
    float recip = recipsf2(100.0);
    __asm__ volatile(
        "mov a2, %1\n"
        "ufloat.s f0, a2, 1\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(data->parameters.o2_voltage)
        : "r"(value), "r"(recip)
        : "a2", "f0", "f1");
}

void read_timing_correction(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(TIMING_CORRECTION_ADDR);
    data->parameters.timing_correction = value;
}

void read_fuel_trim(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(TIMING_CORRECTION_ADDR);
    float recip = recipsf2(1.28);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 128\n"
        "sub a2, a2, a3\n"
        "float.s f0, a2, 1\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(data->parameters.fuel_trim)
        : "r"(value), "r"(recip)
        : "a2", "a3", "f0", "f1");
}

void read_atmosphere_pressure(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(ATMOSPHERIC_PRESSURE_ADDR);
    float recip = recipsf2(0.323);
    __asm__ volatile(
        "movi a2, 930\n"

        "mov a3, %1\n"
        "float.s f0, a3, 1\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"

        "float.s f1, a2, 1\n"
        "sub.s f1, f1, f0\n"
        "rfr %0, f1\n"
        : "=f"(data->parameters.barop)
        : "r"(value), "r"(recip)
        : "a2", "a3", "f0", "f1");
}

void read_input_switches(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(INPUT_SWITCHES_ADDR);
    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"
        // Ignition (bit 7)
        "slli a3, a2, 7\n"
        "and a3, a3, a4\n" // Isolate the bit
        "mov %1, a3\n"
        // Auto Transmission (bit 6)
        "slli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // Test Mode (bit 5)
        "slli a3, a2, 5\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // Read Mode (bit 4)
        "slli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // Neutral (bit 2)
        "slli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // Park (bit 1)
        "slli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        // California (bit 0)
        "slli a3, a2, 0\n"
        "and a3, a3, a4\n"
        "mov %7, a3\n" // Store result in status.california
        : "=r"(data->status0.ignition), "=r"(data->status0.auto_trans), "=r"(data->status0.test_mode),
          "=r"(data->status0.read_mode), "=r"(data->status0.neutral), "=r"(data->status0.park), "=r"(data->status0.california)
        : "r"(value)
        : "a2", "a3", "a4");
}

void read_inout_switches(struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(INOUT_SWITCHES_ADDR);
    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"
        // Idle switch (bit 7)
        "slli a3, a2, 7\n"
        "and a3, a3, a4\n"
        "mov %1, a3\n"
        // AC switch (bit 6)
        "slli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // AC relay (bit 5)
        "slli a3, a2, 5\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // Rad fan (bit 4)
        "slli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // Fuel pump (bit 3)
        "slli a3, a2, 3\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // Purge valve (bit 2)
        "slli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        // Pinging (bit 1)
        "slli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %7, a3\n"
        // Pressure exchange (bit 0)
        "slli a3, a2, 0\n"
        "and a3, a3, a4\n"
        "mov %8, a3\n"
        : "=r"(data->status1.idle_sw), "=r"(data->status1.ac_sw), "=r"(data->status1.ac_relay), "=r"(data->status1.rad_fan),
          "=r"(data->status1.fuel_pump), "=r"(data->status1.purge_valve), "=r"(data->status1.pinging), "=r"(data->status1.press_exch)
        : "r"(value)
        : "a2", "a3", "a4");
}

static void read_trouble_code_one(uint16_t addr, struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(addr);
    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"
        // Crank (bit 7)
        "slli a3, a2, 7\n"
        "and a3, a3, a4\n"
        "mov %1, a3\n"
        // Starter (bit 6)
        "slli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // Cam (bit 5)
        "slli a3, a2, 5\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // Injector one (bit 4)
        "slli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // Injector two (bit 3)
        "slli a3, a2, 3\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // Injector three (bit 2)
        "slli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        // Injector four (bit 1)
        "slli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %7, a3\n"
        : "=r"(data->status2.crank), "=r"(data->status2.starter), "=r"(data->status2.cam), "=r"(data->status2.inj_1),
          "=r"(data->status2.inj_2), "=r"(data->status2.inj_3), "=r"(data->status2.inj_4)
        : "r"(value)
        : "a2", "a3", "a4");
}

void read_active_trouble_code_one(struct state_machine_data *data)
{
    read_trouble_code_one(ACTIVE_TROUBLE_CODE_ONE_ADDR, data);
}

void read_stored_trouble_code_one(struct state_machine_data *data)
{
    read_trouble_code_one(STORED_TROUBLE_CODE_THREE_ADDR, data);
}

static void read_trouble_code_two(uint16_t addr, struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(addr);
    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"
        // Temp (bit 7)
        "slli a3, a2, 7\n"
        "and a3, a3, a4\n"
        "mov %1, a3\n"
        // Knock (bit 6)
        "slli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // MAF (bit 5)
        "slli a3, a2, 5\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // IAC (bit 4)
        "slli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // TPS (bit 3)
        "slli a3, a2, 3\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // O2 (bit 2)
        "slli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        // VSS (bit 1)
        "slli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %7, a3\n"
        // Purge (bit 0)
        "slli a3, a2, 0\n"
        "and a3, a3, a4\n"
        "mov %8, a3\n"
        : "=r"(data->status3.temp), "=r"(data->status3.knock), "=r"(data->status3.maf), "=r"(data->status3.iacv),
          "=r"(data->status3.tps), "=r"(data->status3.oxygen), "=r"(data->status3.vss), "=r"(data->status3.purge)
        : "r"(value)
        : "a2", "a3", "a4");
}

void read_active_trouble_code_two(struct state_machine_data *data)
{
    read_trouble_code_two(ACTIVE_TROUBLE_CODE_TWO_ADDR, data);
}

void read_stored_trouble_code_two(struct state_machine_data *data)
{
    read_trouble_code_two(STORED_TROUBLE_CODE_TWO_ADDR, data);
}

static void read_trouble_code_three(uint16_t addr, struct state_machine_data *data)
{
    uint8_t value = read_data_from_address(addr);
    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"
        // Fuel trim (bit 7)
        "slli a3, a2, 7\n"
        "and a3, a3, a4\n"
        "mov %1, a3\n"
        // Idle switch (bit 6)
        "slli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // WGC (bit 4)
        "slli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // Baro (bit 3)
        "slli a3, a2, 3\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // Wrong MAF (bit 2)
        "slli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // Neutral switch (bit 1)
        "slli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        // Parking switch (bit 0)
        "slli a3, a2, 0\n"
        "and a3, a3, a4\n"
        "mov %7, a3\n"
        : "=r"(data->status4.fuel_trim), "=r"(data->status4.idle_sw), "=r"(data->status4.wgc), "=r"(data->status4.baro),
          "=r"(data->status4.wrong_maf), "=r"(data->status4.neutral_sw), "=r"(data->status4.parking_sw)
        : "r"(value)
        : "a2", "a3", "a4");
}

void read_active_trouble_code_three(struct state_machine_data *data)
{
    read_trouble_code_three(ACTIVE_TROUBLE_CODE_THREE_ADDR, data);
}

void read_stored_trouble_code_three(struct state_machine_data *data)
{
    read_trouble_code_three(STORED_TROUBLE_CODE_THREE_ADDR, data);
}