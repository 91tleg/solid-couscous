#include "decode_trouble_codes.h"

struct input_switches decode_input_switches(uint8_t value)
{
    struct input_switches dtc;
    dtc.ignition   = (value >> 7U) & 1U;
    dtc.auto_trans = (value >> 6U) & 1U;
    dtc.test_mode  = (value >> 5U) & 1U;
    dtc.read_mode  = (value >> 4U) & 1U;
    dtc.neutral    = (value >> 2U) & 1U;
    dtc.park       = (value >> 1U) & 1U;
    dtc.california = value & 1U;
    return dtc;
}

struct io_switches decode_io_switches(uint8_t value)
{
    struct io_switches dtc;
    dtc.idle_sw     = (value >> 7U) & 1U;
    dtc.ac_sw       = (value >> 6U) & 1U;
    dtc.ac_relay    = (value >> 5U) & 1U;
    dtc.rad_fan     = (value >> 4U) & 1U;
    dtc.fuel_pump   = (value >> 3U) & 1U;
    dtc.purge_valve = (value >> 2U) & 1U;
    dtc.pinging     = (value >> 1U) & 1U;
    dtc.press_exch  = value & 1U;
    return dtc;
}

struct trouble_code_one decode_trouble_code_one(uint8_t value)
{
    struct trouble_code_one dtc;
    dtc.crank   = (value >> 7U) & 1U;
    dtc.starter = (value >> 6U) & 1U;
    dtc.cam     = (value >> 5U) & 1U;
    dtc.inj_1   = (value >> 4U) & 1U;
    dtc.inj_2   = (value >> 3U) & 1U;
    dtc.inj_3   = (value >> 2U) & 1U;
    dtc.inj_4   = (value >> 1U) & 1U;
    return dtc;
}

struct trouble_code_two decode_trouble_code_two(uint8_t value)
{
    struct trouble_code_two dtc;
    dtc.temp   = (value >> 7U) & 1U;
    dtc.knock  = (value >> 6U) & 1U;
    dtc.maf    = (value >> 5U) & 1U;
    dtc.iacv   = (value >> 4U) & 1U;
    dtc.tps    = (value >> 3U) & 1U;
    dtc.oxygen = (value >> 2U) & 1U;
    dtc.vss    = (value >> 1U) & 1U;
    dtc.purge  = value & 1U;
    return dtc;
}

struct trouble_code_three decode_trouble_code_three(uint8_t value)
{
    struct trouble_code_three dtc;
    dtc.fuel_trim  = (value >> 7U) & 1U;
    dtc.idle_sw    = (value >> 6U) & 1U;
    dtc.wgc        = (value >> 4U) & 1U;
    dtc.baro       = (value >> 3U) & 1U;
    dtc.wrong_maf  = (value >> 2U) & 1U;
    dtc.neutral_sw = (value >> 1U) & 1U;
    dtc.parking_sw = value & 1U;
    return dtc;
}
