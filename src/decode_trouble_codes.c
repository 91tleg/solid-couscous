#include "decode_binary.h"

void decode_input_switches(uint8_t value, struct input_switches *buf)
{
    __asm__ volatile(
        "mov a2, %0\n"
        "movi a4, 1\n"
        // Ignition (bit 7)
        "slli a3, a2, 7\n"
        "and a3, a3, a4\n"
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
        : "=r"(buf->ignition), "=r"(buf->auto_trans), "=r"(buf->test_mode),
          "=r"(buf->read_mode), "=r"(buf->neutral), "=r"(buf->park), "=r"(buf->california)
        : "r"(value)
        : "a2", "a3", "a4"
    );
}

void decode_io_switches(uint8_t value, struct io_switches *buf)
{
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
        : "=r"(buf->idle_sw), "=r"(buf->ac_sw), "=r"(buf->ac_relay), "=r"(buf->rad_fan),
          "=r"(buf->fuel_pump), "=r"(buf->purge_valve), "=r"(buf->pinging), "=r"(buf->press_exch)
        : "r"(value)
        : "a2", "a3", "a4"
    );
}

void decode_trouble_code_one(uint8_t value, struct trouble_code_one *buf)
{
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
        : "=r"(buf->crank), "=r"(buf->starter), "=r"(buf->cam), "=r"(buf->inj_1),
          "=r"(buf->inj_2), "=r"(buf->inj_3), "=r"(buf->inj_4)
        : "r"(value)
        : "a2", "a3", "a4"
    );
}

void decode_trouble_code_two(uint8_t value, struct trouble_code_two *buf)
{
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
        : "=r"(buf->temp), "=r"(buf->knock), "=r"(buf->maf), "=r"(buf->iacv),
          "=r"(buf->tps), "=r"(buf->oxygen), "=r"(buf->vss), "=r"(buf->purge)
        : "r"(value)
        : "a2", "a3", "a4"
    );
}

void decode_trouble_code_three(uint8_t value, struct trouble_code_three *buf)
{
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
        : "=r"(buf->fuel_trim), "=r"(buf->idle_sw), "=r"(buf->wgc), "=r"(buf->baro),
          "=r"(buf->wrong_maf), "=r"(buf->neutral_sw), "=r"(buf->parking_sw)
        : "r"(value)
        : "a2", "a3", "a4"
    );
}