#include "decode_trouble_codes.h"

union param_type decode_input_switches(uint8_t value)
{
    union param_type dtc;
    __asm__ volatile(
        "mov a2, %7\n"
        "movi a4, 1\n"
        // Ignition (bit 7)
        "srli a3, a2, 7\n"
        "and a3, a3, a4\n"
        "mov %0, a3\n"
        // Auto Transmission (bit 6)
        "srli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %1, a3\n"
        // Test Mode (bit 5)
        "srli a3, a2, 5\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // Read Mode (bit 4)
        "srli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // Neutral (bit 2)
        "srli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // Park (bit 1)
        "srli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // California (bit 0)
        "srli a3, a2, 0\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        : "=r"(dtc.in_sw.ignition), "=r"(dtc.in_sw.auto_trans),
          "=r"(dtc.in_sw.test_mode), "=r"(dtc.in_sw.read_mode),
          "=r"(dtc.in_sw.neutral), "=r"(dtc.in_sw.park),
          "=r"(dtc.in_sw.california)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}

union param_type decode_io_switches(uint8_t value)
{
    union param_type dtc;
    __asm__ volatile(
        "mov a2, %8\n"
        "movi a4, 1\n"
        // Idle switch (bit 7)
        "srli a3, a2, 7\n"
        "and a3, a3, a4\n"
        "mov %0, a3\n"
        // AC switch (bit 6)
        "srli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %1, a3\n"
        // AC relay (bit 5)
        "srli a3, a2, 5\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // Rad fan (bit 4)
        "srli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // Fuel pump (bit 3)
        "srli a3, a2, 3\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // Purge valve (bit 2)
        "srli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // Pinging (bit 1)
        "srli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        // Pressure exchange (bit 0)
        "srli a3, a2, 0\n"
        "and a3, a3, a4\n"
        "mov %7, a3\n"
        : "=r"(dtc.io_sw.idle_sw), "=r"(dtc.io_sw.ac_sw),
          "=r"(dtc.io_sw.ac_relay), "=r"(dtc.io_sw.rad_fan),
          "=r"(dtc.io_sw.fuel_pump), "=r"(dtc.io_sw.purge_valve),
          "=r"(dtc.io_sw.pinging), "=r"(dtc.io_sw.press_exch)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}

union param_type decode_trouble_code_one(uint8_t value)
{
    union param_type dtc;
    __asm__ volatile(
        "mov a2, %7\n"
        "movi a4, 1\n"
        // Crank (bit 7)
        "srli a3, a2, 7\n"
        "and a3, a3, a4\n"
        "mov %0, a3\n"
        // Starter (bit 6)
        "srli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %1, a3\n"
        // Cam (bit 5)
        "srli a3, a2, 5\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // Injector one (bit 4)
        "srli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // Injector two (bit 3)
        "srli a3, a2, 3\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // Injector three (bit 2)
        "srli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // Injector four (bit 1)
        "srli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        : "=r"(dtc.tc_one.crank), "=r"(dtc.tc_one.starter),
          "=r"(dtc.tc_one.cam), "=r"(dtc.tc_one.inj_1),
          "=r"(dtc.tc_one.inj_2), "=r"(dtc.tc_one.inj_3),
          "=r"(dtc.tc_one.inj_4)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}

union param_type decode_trouble_code_two(uint8_t value)
{
    union param_type dtc;
    __asm__ volatile(
        "mov a2, %8\n"
        "movi a4, 1\n"
        // Temp (bit 7)
        "srli a3, a2, 7\n"
        "and a3, a3, a4\n"
        "mov %0, a3\n"
        // Knock (bit 6)
        "srli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %1, a3\n"
        // MAF (bit 5)
        "srli a3, a2, 5\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // IAC (bit 4)
        "srli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // TPS (bit 3)
        "srli a3, a2, 3\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // O2 (bit 2)
        "srli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // VSS (bit 1)
        "srli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        // Purge (bit 0)
        "srli a3, a2, 0\n"
        "and a3, a3, a4\n"
        "mov %7, a3\n"
        : "=r"(dtc.tc_two.temp), "=r"(dtc.tc_two.knock), 
          "=r"(dtc.tc_two.maf), "=r"(dtc.tc_two.iacv),
          "=r"(dtc.tc_two.tps), "=r"(dtc.tc_two.oxygen), 
          "=r"(dtc.tc_two.vss), "=r"(dtc.tc_two.purge)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}

union param_type decode_trouble_code_three(uint8_t value)
{
    union param_type dtc;
    __asm__ volatile(
        "mov a2, %7\n"
        "movi a4, 1\n"
        // Fuel trim (bit 7)
        "srli a3, a2, 7\n"
        "and a3, a3, a4\n"
        "mov %0, a3\n"
        // Idle switch (bit 6)
        "srli a3, a2, 6\n"
        "and a3, a3, a4\n"
        "mov %1, a3\n"
        // WGC (bit 4)
        "srli a3, a2, 4\n"
        "and a3, a3, a4\n"
        "mov %2, a3\n"
        // Baro (bit 3)
        "srli a3, a2, 3\n"
        "and a3, a3, a4\n"
        "mov %3, a3\n"
        // Wrong MAF (bit 2)
        "srli a3, a2, 2\n"
        "and a3, a3, a4\n"
        "mov %4, a3\n"
        // Neutral switch (bit 1)
        "srli a3, a2, 1\n"
        "and a3, a3, a4\n"
        "mov %5, a3\n"
        // Parking switch (bit 0)
        "srli a3, a2, 0\n"
        "and a3, a3, a4\n"
        "mov %6, a3\n"
        : "=r"(dtc.tc_three.fuel_trim), "=r"(dtc.tc_three.idle_sw), 
          "=r"(dtc.tc_three.wgc), "=r"(dtc.tc_three.baro),
          "=r"(dtc.tc_three.wrong_maf), "=r"(dtc.tc_three.neutral_sw), 
          "=r"(dtc.tc_three.parking_sw)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}