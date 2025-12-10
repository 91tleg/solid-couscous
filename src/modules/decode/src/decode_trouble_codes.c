#include "decode_trouble_codes.h"

struct input_switches decode_input_switches(uint8_t value)
{
    struct input_switches dtc;
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
        : "=r"(dtc.ignition), "=r"(dtc.auto_trans),
          "=r"(dtc.test_mode), "=r"(dtc.read_mode),
          "=r"(dtc.neutral), "=r"(dtc.park),
          "=r"(dtc.california)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}

struct io_switches decode_io_switches(uint8_t value)
{
    struct io_switches dtc;
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
        : "=r"(dtc.idle_sw), "=r"(dtc.ac_sw),
          "=r"(dtc.ac_relay), "=r"(dtc.rad_fan),
          "=r"(dtc.fuel_pump), "=r"(dtc.purge_valve),
          "=r"(dtc.pinging), "=r"(dtc.press_exch)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}

struct trouble_code_one decode_trouble_code_one(uint8_t value)
{
    struct trouble_code_one dtc;
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
        : "=r"(dtc.crank), "=r"(dtc.starter),
          "=r"(dtc.cam), "=r"(dtc.inj_1),
          "=r"(dtc.inj_2), "=r"(dtc.inj_3),
          "=r"(dtc.inj_4)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}

struct trouble_code_two decode_trouble_code_two(uint8_t value)
{
    struct trouble_code_two dtc;
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
        : "=r"(dtc.temp), "=r"(dtc.knock), 
          "=r"(dtc.maf), "=r"(dtc.iacv),
          "=r"(dtc.tps), "=r"(dtc.oxygen), 
          "=r"(dtc.vss), "=r"(dtc.purge)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}

struct trouble_code_three decode_trouble_code_three(uint8_t value)
{
    struct trouble_code_three dtc;
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
        : "=r"(dtc.fuel_trim), "=r"(dtc.idle_sw), 
          "=r"(dtc.wgc), "=r"(dtc.baro),
          "=r"(dtc.wrong_maf), "=r"(dtc.neutral_sw), 
          "=r"(dtc.parking_sw)
        : "r"(value)
        : "a2", "a3", "a4");
    return dtc;
}