#include "deocde_parameters.h"
#include <esp_attr.h>

const uint8_t coolant_lookup_table[] __attribute__((section(".rodata"))) =
{
	// the first 14 values need 255 added
	146,132,117,105, 90, 76, 63, 48, 36, 29, 22, 15,  9,  2,
	                                                        250,243,
	237,234,230,226,223,219,216,212,208,205,203,199,198,194,192,189,
	187,185,183,181,180,178,176,174,172,171,169,167,165,163,162,160,
	158,156,154,153,153,151,149,147,147,145,144,144,142,140,140,138,
	138,136,135,135,133,131,131,129,129,127,127,126,126,124,124,122,
	122,120,120,118,118,117,117,115,115,113,113,111,111,109,109,108,
	108,106,106,104,104,102,102,100,100, 99, 99, 97, 97, 97, 95, 95,
	 95, 93, 93, 91, 91, 90, 90, 88, 88, 86, 86, 84, 84, 84, 82, 82,
     82, 81, 81, 79, 79, 79, 77, 77, 77, 75, 75, 73, 73, 73, 72, 72,
	 72, 70, 70, 68, 68, 68, 66, 66, 66, 64, 64, 63, 63, 61, 61, 59,
     59, 57, 57, 55, 55, 55, 54, 54, 54, 52, 52, 50, 50, 50, 48, 48,
	 48, 46, 46, 45, 45, 43, 43, 41, 41, 39, 39, 37, 37, 37, 36, 36,
	 36, 34, 34, 32, 32, 30, 30, 28, 28, 27, 27, 25, 25, 23, 23, 21,
	 21, 19, 18, 18, 16, 14, 14, 12, 12, 10,  9,  9,  7,  5,  5,  3,
	  3,  1,  0,
	              2,  4,  6,  8,  9,  9, 11, 13, 15, 17, 18, 20, 22,
	 24, 27, 31, 33, 36, 40, 42, 45, 47, 47, 47, 47, 47, 47, 47, 47
	// the last 29 values need to be inverted
};

float IRAM_ATTR recipsf2(float a)
{
    float result;
    __asm__ volatile(
        "wfr f1, %1\n"         // Write from register
        "recip0.s f0, f1\n"    // Store the recip in f0
        "const.s f2, 1\n"      // Load 1 into f1
        "msub.s f2, f1, f0\n"  // f2 = f2 - (f1 * f0)
        "maddn.s f0, f0, f2\n" // f0 = f0 + (f0 * f2)
        "const.s f2, 1\n"      // Load 1 into f1 again
        "msub.s f2, f1, f0\n"  // f2 = f2 - (f1 * f0)
        "maddn.s f0, f0, f2\n" // f0 = f0 + (f0 * f2)
        "rfr %0, f0\n"         // Read from register
        : "=r"(result)
        : "r"(a)
        : "f0", "f1", "f2");
    return result;
}

float decode_battery_voltage(uint8_t value)
{
    float result;
    float recip = recipsf2(100.0f);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 8\n"
        "mull a2, a2, a3\n"
        "ufloat.s f0, a2, 0\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(result)
        : "r"(value), "r"(recip)
        : "a2", "a3", "f0", "f1");
    return result;
}

uint8_t decode_vehicle_speed(uint8_t value)
{
    uint8_t result;
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 10\n"
        "mull a2, a2, a3\n"
        "srli a2, a2, 4\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
}

uint16_t decode_engine_speed(uint8_t value)
{
    uint16_t result;
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 25\n"
        "mull a2, a2, a3\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
}

int16_t decode_coolant_temp(uint8_t value)
{
    int16_t result;
    uint8_t table_value = coolant_lookup_table[value];
    __asm__ volatile(
        "mov a2, %1\n"
        "mov a3, %2\n"
        "movi a4, 227\n"
        "bgeu a2, a4, .Lneg%=\n" // Branch if Greater Than or Eq Unsigned
        "movi a4, 14\n"
        "bltu a2, a4, .Ladd_255%=\n" // Branch if Less Than Unsigned
        "j .Ldone%=\n"

        ".Lneg%=:\n"
        "neg a3, a3\n"
        "j .Ldone%=\n"

        ".Ladd_255%=:\n"
        "addi a3, a3, 255\n"
        "j .Ldone%=\n"

        ".Ldone%=:\n"
        "mov %0, a3\n"
        : "=r"(result)
        : "r"(value), "r"(table_value)
        : "a2", "a3", "a4");
    return result;
}

float decode_airflow(uint8_t value)
{
    float result;
    float recip = recipsf2(50.0f);
    __asm__ volatile(
        "mov a2, %1\n"
        "ufloat.s f0, a2, 0\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(result)
        : "r"(value), "r"(recip)
        : "a2", "f0", "f1");
    return result;
}

uint8_t decode_throttle_percentage(uint8_t value)
{
    uint8_t result;
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 100\n"
        "mull a2, a2, a3\n"
        "srli a2, a2, 8\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
}

float decode_throttle_signal(uint8_t value)
{
    float result;
    float recip = recipsf2(50.0f);
    __asm__ volatile(
        "mov a2, %1\n"
        "ufloat.s f0, a2, 0\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(result)
        : "r"(value), "r"(recip)
        : "a2", "f0", "f1");
    return result;
}

float IRAM_ATTR decode_manifold_pressure(uint8_t value)
{
    float result;
    float vacuum_scale = 0.0030757f;
    float boost_scale = 0.0015107f;
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 100\n"
        "mull a2, a2, a3\n"
        "movi a3, 13568\n"
        "sub a2, a2, a3\n"
        "bltz a2, .Lvacuum%=\n"
        "j .Lboost%=\n"

        ".Lvacuum%=:\n"
        "float.s f0, a2, 0\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "j .Ldone%=\n"

        ".Lboost%=:\n"
        "float.s f0, a2, 0\n"
        "wfr f1, %3\n"
        "mul.s f0, f0, f1\n"
        "j .Ldone%=\n"

        ".Ldone%=:\n"
        "rfr %0, f0\n"
        : "=r"(result)
        : "r"(value), "r"(vacuum_scale), "r"(boost_scale)
        : "a2", "a3", "f0", "f1");
    return result;
}

uint8_t decode_boost_control_duty_cycle(uint8_t value)
{
    uint8_t result;
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 100\n"
        "mull a2, a2, a3\n"
        "srli a2, a2, 8\n"
        "mov %0, a2\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "a3");
    return result;
}

uint8_t decode_ignition_timing(uint8_t value)
{
    return value;
}

uint8_t decode_engine_load(uint8_t value)
{
    return value;
}

float decode_injector_pulse_width(uint8_t value)
{
    float result;
    float recip = recipsf2(7.8f);
    __asm__ volatile(
        "mov a2, %1\n"
        "ufloat.s f0, a2, 0\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(result)
        : "r"(value), "r"(recip)
        : "a2", "f0", "f1");
    return result;
}

float decode_iacv_duty_cycle(uint8_t value)
{
    float result;
    __asm__ volatile(
        "mov a2, %1\n"
        "ufloat.s f0, a2, 1\n"
        "rfr %0, f0\n"
        : "=r"(result)
        : "r"(value)
        : "a2", "f0");
    return result;
}

float decode_o2_signal(uint8_t value)
{
    float result;
    float recip = recipsf2(100.0f);
    __asm__ volatile(
        "mov a2, %1\n"
        "ufloat.s f0, a2, 0\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(result)
        : "r"(value), "r"(recip)
        : "a2", "f0", "f1");
    return result;
}

uint8_t decode_timing_correction(uint8_t value)
{
    return value;
}

float decode_fuel_trim(uint8_t value)
{
    float result;
    float recip = recipsf2(1.28f);
    __asm__ volatile(
        "mov a2, %1\n"
        "movi a3, 128\n"
        "sub a2, a2, a3\n"
        "float.s f0, a2, 0\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(result)
        : "r"(value), "r"(recip)
        : "a2", "a3", "f0", "f1");
    return result;
}

float decode_atmosphere_pressure(uint8_t value)
{
    float result;
    __asm__ volatile(
        "mov a2, %1\n"
        "float.s f0, a2, 0\n"
        "wfr f1, %2\n"
        "mul.s f0, f0, f1\n"
        "wfr f1, %3\n"
        "add.s f0, f0, f1\n"
        "rfr %0, f0\n"
        : "=r"(result)
        : "r"(value), "r"(1.25f), "r"(500.0f)
        : "a2", "f0", "f1");
    return result;
}
