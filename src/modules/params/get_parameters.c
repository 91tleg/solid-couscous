#include "get_parameters.h"
#include "process_parameters.h"
#include "decode_trouble_codes.h"
#include "defines.h"
#include "modules/ecu/ssm1.h"
#include <string.h>

#define MAX_PARAMS  (26)

static struct read_ctx ctx_pool[MAX_PARAMS] = {0};
static struct romid_ctx romid_ctx = {0};
static bool romid_initialized = false;

static struct read_ctx *get_ctx(state_e state, uint16_t addr)
{
    struct read_ctx *ctx = &ctx_pool[state];

    if (!ctx->used)
    {
        ctx->used = true;
        ctx->state = state;
        ctx->addr = addr;
        ctx->data = 0;
        ctx->sent = false;
        ctx->read_state = READ_WAIT_MSB;
    }

    return ctx;
}

typedef union param_type (*decoder_func_t)(uint8_t value);

struct param_entry
{
    state_e state;
    uint16_t addr;
    decoder_func_t decoder;
};

static const struct param_entry param_lut[] __attribute__((section(".rodata"))) =
    {
        {STATE_BATTERY_V, BATTERY_VOLTAGE_ADDR, decode_battery_voltage},
        {STATE_VEHICLE_SPEED, VEHICLE_SPEED_ADDR, decode_vehicle_speed},
        {STATE_ENGINE_SPEED, ENGINE_SPEED_ADDR, decode_engine_speed},
        {STATE_COOLANT_TEMP, COOLANT_TEMP_ADDR, decode_coolant_temp},
        {STATE_AIRFLOW, AIRFLOW_SENSOR_ADDR, decode_airflow},
        {STATE_THROTTLE, THROTTLE_ADDR, decode_throttle_percentage},
        {STATE_THROTTLE_V, THROTTLE_ADDR, decode_throttle_signal},
        {STATE_MANIP, MANIFOLD_PRESSURE_ADDR, decode_manifold_pressure},
        {STATE_BOOST_SOLINOID, BOOST_SOLENOID_ADDR, decode_boost_control_duty_cycle},
        {STATE_IGNITION_TIMING, IGNITION_ADVANCE_ADDR, decode_ignition_timing},
        {STATE_LOAD, ENGINE_LOAD_ADDR, decode_engine_load},
        {STATE_INJECTOR_PW, INJECTOR_PULSE_WIDTH_ADDR, decode_injector_pulse_width},
        {STATE_IAC, IAC_VALVE_ADDR, decode_iacv_duty_cycle},
        {STATE_O2_V, O2_AVERAGE_ADDR, decode_o2_signal},
        {STATE_TIMING_CORRECTION, TIMING_CORRECTION_ADDR, decode_timing_correction},
        {STATE_FUEL_TRIM, AF_CORRECTION_ADDR, decode_fuel_trim},
        {STATE_BAROP, ATMOSPHERIC_PRESSURE_ADDR, decode_atmosphere_pressure},
        {STATE_INPUT_SWITCHES,INPUT_SWITCHES_ADDR, decode_input_switches},
        {STATE_INOUT_SWITCHES, INOUT_SWITCHES_ADDR, decode_io_switches},
        {STATE_ACTIVE_CODE_ONE, ACTIVE_TROUBLE_CODE_ONE_ADDR, decode_trouble_code_one},
        {STATE_ACTIVE_CODE_TWO, ACTIVE_TROUBLE_CODE_TWO_ADDR, decode_trouble_code_two},
        {STATE_ACTIVE_CODE_THREE, ACTIVE_TROUBLE_CODE_THREE_ADDR, decode_trouble_code_three},
        {STATE_STORED_CODE_ONE, ACTIVE_TROUBLE_CODE_ONE_ADDR, decode_trouble_code_one},
        {STATE_STORED_CODE_TWO, ACTIVE_TROUBLE_CODE_TWO_ADDR, decode_trouble_code_two},
        {STATE_STORED_CODE_THREE, ACTIVE_TROUBLE_CODE_THREE_ADDR, decode_trouble_code_three}
    };

void get_romid(uint8_t *romid_out)
{
    if (!romid_initialized)
    {
        memset(&romid_ctx, 0, sizeof(romid_ctx));
        romid_ctx.state = ROMID_WAIT_FF;
        romid_ctx.done = false;
        romid_initialized = true;
    }

    read_romid(&romid_ctx);

    if (romid_ctx.done)
    {
        for (int i = 0; i < 3; i++)
            romid_out[i] = romid_ctx.romid_out[i];
    }
}

union param_type get_param(state_e state)
{
    for (size_t i = 0; i < ARRAY_SIZE(param_lut); i++)
    {
        if (param_lut[i].state == state)
        {
            struct read_ctx *ctx = get_ctx(state, param_lut[i].addr);
            if (!ctx)
            {
                return (union param_type){0};
            }

            read_data_from_address(ctx);
            return param_lut[i].decoder(ctx->data);
        }
    }
    return (union param_type){0};
}

void reset_ctx_sent_flag(state_e state)
{
    struct read_ctx *ctx = &ctx_pool[state];
    if (ctx->used)
    {
        ctx->sent = false;
    }
}