#include "decode_task.h"
#include "decode_parameters.h"
#include "decode_trouble_codes.h"
#include "log.h"
#include "uart_data.h"
#include "fsm_data.h"
#include "fsm_states.h"
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#define TAG             "DecodeTask"
#define TASK_STACK_SIZE (4096U)

static StaticTask_t decode_task_tcb;
static StackType_t decode_task_stack[TASK_STACK_SIZE];
static QueueHandle_t uart_data_queue;
static QueueHandle_t display_queue;

static void decode_task(void *parameters)
{
    struct uart_data d = {0};
    struct fsm_data sm_data = {0};

    for (;;)
    {
        xQueueReceive(uart_data_queue, &d, portMAX_DELAY);

        sm_data.state = d.state;
        switch (d.state)
        {
        case STATE_ROMID:
            memcpy(sm_data.params.romid, d.romid, sizeof(d.romid));
            break;

        case STATE_BATTERY_V:
            sm_data.params.battery_voltage = decode_battery_voltage(d.value);
            break;

        case STATE_VEHICLE_SPEED:
            sm_data.params.vehicle_speed = decode_vehicle_speed(d.value);
            break;

        case STATE_ENGINE_SPEED:
            sm_data.params.engine_speed = decode_engine_speed(d.value);
            break;

        case STATE_COOLANT_TEMP:
            sm_data.params.coolant_temp = decode_coolant_temp(d.value);
            break;

        case STATE_AIRFLOW:
            sm_data.params.airflow = decode_airflow(d.value);
            break;

        case STATE_THROTTLE:
            sm_data.params.throttle_percentage = decode_throttle_percentage(d.value);
            break;

        case STATE_THROTTLE_V:
            sm_data.params.throttle_signal = decode_throttle_signal(d.value);
            break;

        case STATE_MANIP:
            sm_data.params.manip = decode_manifold_pressure(d.value);
            break;

        case STATE_BOOST_SOLINOID:
            sm_data.params.boost_solenoid = decode_boost_control_duty_cycle(d.value);
            break;

        case STATE_IGNITION_TIMING:
            sm_data.params.ignition_timing = decode_ignition_timing(d.value);
            break;

        case STATE_LOAD:
            sm_data.params.engine_load = decode_engine_load(d.value);
            break;

        case STATE_INJECTOR_PW:
            sm_data.params.injector_pw = decode_injector_pulse_width(d.value);
            break;

        case STATE_IAC:
            sm_data.params.iac = decode_iacv_duty_cycle(d.value);
            break;

        case STATE_O2_V:
            sm_data.params.o2_signal = decode_o2_signal(d.value);
            break;

        case STATE_TIMING_CORRECTION:
            sm_data.params.timing_correction = decode_timing_correction(d.value);
            break;

        case STATE_FUEL_TRIM:
            sm_data.params.fuel_trim = decode_fuel_trim(d.value);
            break;

        case STATE_BAROP:
            sm_data.params.barop = decode_atmosphere_pressure(d.value);
            break;

        case STATE_INPUT_SWITCHES:
            sm_data.in_sw = decode_input_switches(d.value);
            break;

        case STATE_INOUT_SWITCHES:
            sm_data.io_sw = decode_io_switches(d.value);
            break;

        case STATE_ACTIVE_CODE_ONE:
        case STATE_STORED_CODE_ONE:
            sm_data.tc1 = decode_trouble_code_one(d.value);
            break;

        case STATE_ACTIVE_CODE_TWO:
        case STATE_STORED_CODE_TWO:
            sm_data.tc2 = decode_trouble_code_two(d.value);
            break;

        case STATE_ACTIVE_CODE_THREE:
        case STATE_STORED_CODE_THREE:
            sm_data.tc3 = decode_trouble_code_three(d.value);
            break;
        }

        xQueueSend(display_queue, &sm_data, 0);
    }
}

void decode_task_init(QueueHandle_t uart_data_q, QueueHandle_t display_q)
{
    uart_data_queue = uart_data_q;
    display_queue = display_q;

    xTaskCreateStatic(
        decode_task,
        TAG,
        TASK_STACK_SIZE,
        NULL,
        2,
        decode_task_stack,
        &decode_task_tcb
    );
    LOGI(TAG, "Task started");
}