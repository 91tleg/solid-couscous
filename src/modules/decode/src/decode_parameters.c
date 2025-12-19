#include "decode_parameters.h"

const uint8_t coolant_lookup_table[] =
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

#if defined(__XTENSA__) && defined(CONFIG_FAST_DIVF)
/**
 * Fast replacement for __divsf3 on Xtensa.
 * - Uses reciprocal approximation + 2x Newton–Raphson
 * - ~1–2 ulp relative error
 * - Not IEEE754 compliant
 * - Undefined behavior for NaN, Inf, or b == 0
 **/
float __divsf3(float a, float b)
{
    float inversed_b;
    float tmp;
    __asm__ volatile (
        "recip0.s %0, %2\n"
        "const.s %1, 1\n"
        "msub.s %1, %2, %0\n"
        "madd.s %0, %0, %1\n"
        "const.s %1, 1\n"
        "msub.s %1, %2, %0\n"
        "maddn.s %0, %0, %1\n"
        :"=&f"(inversed_b),"=&f"(tmp)
        :"f"(b)
    );
    
    return a * inversed_b;
}
#endif

float decode_battery_voltage(uint8_t value)
{
    return (float)(value << 3U) * 0.01f;
}

uint8_t decode_vehicle_speed(uint8_t value)
{
    return (uint8_t)(((uint16_t)(value * 10U)) >> 4U);
}

uint16_t decode_engine_speed(uint8_t value)
{
    return value * 25U;
}

int16_t decode_coolant_temp(uint8_t value)
{
    uint8_t table_value = coolant_lookup_table[value];
    int16_t result = (int16_t)table_value;

    if (value >= 227)
    {
        return -result;
    }
    else if (value < 14)
    {
        return result + 255;
    }

    return result;
}

float decode_airflow(uint8_t value)
{
    return (float)value * 0.02f;
}

uint8_t decode_throttle_percentage(uint8_t value)
{
    return (uint8_t)(((uint16_t)value * 100U) >> 8U);
}

float decode_throttle_signal(uint8_t value)
{
    return (float)value * 0.02f;
}

float decode_manifold_pressure(uint8_t value)
{
    int32_t x = (int32_t)value * 100 - 13568;
    if (x < 0)
    {
        return (float)x * 0.0030757f; // Vacuum
    }
    else
    {
        return (float)x * 0.0015107f; // Boost
    }
}

uint8_t decode_boost_control_duty_cycle(uint8_t value)
{
    return (uint8_t)(((uint16_t)value * 100U) >> 8U);
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
    return (float)value * 0.128f;
}

float decode_iacv_duty_cycle(uint8_t value)
{
    return (float)value * 0.5f;
}

float decode_o2_signal(uint8_t value)
{
    return (float)value * 0.01f;
}

uint8_t decode_timing_correction(uint8_t value)
{
    return value;
}

float decode_fuel_trim(uint8_t value)
{
    return (float)((int16_t)value - 128) / 1.28f;
}

float decode_atmosphere_pressure(uint8_t value)
{
    return (float)value * 1.25f + 500.f;
}
