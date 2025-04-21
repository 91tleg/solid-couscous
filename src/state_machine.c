#include "state_machine.h"
#include "string.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "defines.h"
#include "process_parameters.h"
#include "button.h"
#include "uart.h"

QueueHandle_t lcd_queue;

struct state_transition
{
    state_e from;
    state_event_e event; // Event that triggers the transition
    state_e to;
};

static const struct state_transition state_transitions[] __attribute__((section(".rodata"))) =
    {
        {STATE_ROMID, STATE_EVENT_BUTTON_PRESS, STATE_BATTERY_V},
        {STATE_ROMID, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_BATTERY_V, STATE_EVENT_BUTTON_PRESS, STATE_VEHICLE_SPEED},
        {STATE_BATTERY_V, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_VEHICLE_SPEED, STATE_EVENT_BUTTON_PRESS, STATE_ENGINE_SPEED},
        {STATE_VEHICLE_SPEED, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_ENGINE_SPEED, STATE_EVENT_BUTTON_PRESS, STATE_COOLANT_TEMP},
        {STATE_ENGINE_SPEED, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_COOLANT_TEMP, STATE_EVENT_BUTTON_PRESS, STATE_AIRFLOW},
        {STATE_COOLANT_TEMP, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_AIRFLOW, STATE_EVENT_BUTTON_PRESS, STATE_THROTTLE},
        {STATE_AIRFLOW, STATE_EVENT_BUTTON_LONG_PRESS, STATE_AIRFLOW},
        {STATE_THROTTLE, STATE_EVENT_BUTTON_PRESS, STATE_THROTTLE_V},
        {STATE_THROTTLE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_THROTTLE_V, STATE_EVENT_BUTTON_PRESS, STATE_MANIP},
        {STATE_THROTTLE_V, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_MANIP, STATE_EVENT_BUTTON_PRESS, STATE_BOOST_SOLINOID},
        {STATE_MANIP, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_BOOST_SOLINOID, STATE_EVENT_BUTTON_PRESS, STATE_IGNITION_TIMING},
        {STATE_BOOST_SOLINOID, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_IGNITION_TIMING, STATE_EVENT_BUTTON_PRESS, STATE_LOAD},
        {STATE_IGNITION_TIMING, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_LOAD, STATE_EVENT_BUTTON_PRESS, STATE_INJECTOR_PW},
        {STATE_LOAD, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_INJECTOR_PW, STATE_EVENT_BUTTON_PRESS, STATE_IAC},
        {STATE_INJECTOR_PW, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_IAC, STATE_EVENT_BUTTON_PRESS, STATE_O2_V},
        {STATE_IAC, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_O2_V, STATE_EVENT_BUTTON_PRESS, STATE_TIMING_CORRECTION},
        {STATE_O2_V, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_TIMING_CORRECTION, STATE_EVENT_BUTTON_PRESS, STATE_FUEL_TRIM},
        {STATE_TIMING_CORRECTION, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_FUEL_TRIM, STATE_EVENT_BUTTON_PRESS, STATE_BAROP},
        {STATE_FUEL_TRIM, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_BAROP, STATE_EVENT_BUTTON_PRESS, STATE_INPUT_SWITCHES},
        {STATE_BAROP, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_INPUT_SWITCHES, STATE_EVENT_BUTTON_PRESS, STATE_INOUT_SWITCHES},
        {STATE_INPUT_SWITCHES, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_INOUT_SWITCHES, STATE_EVENT_BUTTON_PRESS, STATE_ACTIVE_CODE_ONE},
        {STATE_INOUT_SWITCHES, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_ACTIVE_CODE_ONE, STATE_EVENT_BUTTON_PRESS, STATE_ACTIVE_CODE_TWO},
        {STATE_ACTIVE_CODE_ONE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},
        {STATE_ACTIVE_CODE_TWO, STATE_EVENT_BUTTON_PRESS, STATE_ACTIVE_CODE_THREE},
        {STATE_ACTIVE_CODE_TWO, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID}, 
        {STATE_ACTIVE_CODE_THREE, STATE_EVENT_BUTTON_PRESS, STATE_STORED_CODE_ONE},
        {STATE_ACTIVE_CODE_THREE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID}, 
        {STATE_STORED_CODE_ONE, STATE_EVENT_BUTTON_PRESS, STATE_STORED_CODE_TWO},
        {STATE_STORED_CODE_ONE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},  
        {STATE_STORED_CODE_TWO, STATE_EVENT_BUTTON_PRESS, STATE_STORED_CODE_THREE},
        {STATE_STORED_CODE_TWO, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID},  
        {STATE_STORED_CODE_THREE, STATE_EVENT_BUTTON_PRESS, STATE_ROMID},
        {STATE_STORED_CODE_THREE, STATE_EVENT_BUTTON_LONG_PRESS, STATE_ROMID}
};

static inline void state_machine_init(struct state_machine_data *data)
{
    data->state = STATE_ROMID;
    memset(&data->parameters, 0, sizeof(data->parameters));
    memset(&data->status0, 0, sizeof(data->status0));
    memset(&data->status1, 0, sizeof(data->status1));
    memset(&data->status2, 0, sizeof(data->status2));
    memset(&data->status3, 0, sizeof(data->status3));
    memset(&data->status4, 0, sizeof(data->status4));
}

static void state_enter(struct state_machine_data *data, state_e from, state_event_e event, state_e to)
{
    if (from != to)
    {
        data->state = to;
    }
    switch (data->state)
    {
    case STATE_ROMID:
        ESP_LOGI("SM", "ROMID");
        if(data->parameters.romid[0] == 0x00)
        {
            //get_romid(data->parameters.romid);
        }
        break;
    case STATE_BATTERY_V:
        //read_battery_voltage(data);
        ESP_LOGI("SM", "BATV: %f", data->parameters.battery_voltage);
        break;
    case STATE_VEHICLE_SPEED:
        //read_vehicle_speed(data);
        ESP_LOGI("SM", "SPD: %f", data->parameters.vehicle_speed);
        break;
    case STATE_ENGINE_SPEED:
        //read_engine_speed(data);
        ESP_LOGI("SM", "RPM: %u", data->parameters.engine_speed);
        break;
    case STATE_COOLANT_TEMP:
        //read_coolant_temp(data);
        ESP_LOGI("SM", "COOLANT: %d", data->parameters.coolant_temp);
        break;
    case STATE_AIRFLOW:
        read_airflow(data);
        ESP_LOGI("SM", "MAF: %f", data->parameters.airflow);
        break;
    case STATE_THROTTLE:
        read_throttle_percentage(data);
        ESP_LOGI("SM", "THROTTLE: %u", data->parameters.throttle_percentage);
        break;
    case STATE_THROTTLE_V:
        read_throttle_signal(data);
        ESP_LOGI("SM", "THROTTLEV: %f", data->parameters.throttle_voltage);
        break;
    case STATE_MANIP:
        read_manifold_pressure(data);
        ESP_LOGI("SM", "MANIP: %f", data->parameters.manip);
        break;
    case STATE_BOOST_SOLINOID:
        read_boost_control_duty_cycle(data);
        ESP_LOGI("SM", "WGC: %f", data->parameters.boost_solenoid);
        break;
    case STATE_IGNITION_TIMING:
        read_ignition_timing(data);
        ESP_LOGI("SM", "IGN: %u", data->parameters.ignition_timing);
        break;
    case STATE_LOAD:
        read_load(data);
        ESP_LOGI("SM", "LOAD: %u", data->parameters.load);
        break;
    case STATE_INJECTOR_PW:
        read_injector_pulse_width(data);
        ESP_LOGI("SM", "INJ: %f", data->parameters.injector_pw);
        break;
    case STATE_IAC:
        read_iacv_duty_cycle(data);
        ESP_LOGI("SM", "IAC: %u", data->parameters.iac);
        break;
    case STATE_O2_V:
        read_o2_signal(data);
        ESP_LOGI("SM", "O2V: %f", data->parameters.o2_voltage);
        break;
    case STATE_TIMING_CORRECTION:
        read_timing_correction(data);
        ESP_LOGI("SM", "TIM: %u", data->parameters.timing_correction);
        break;
    case STATE_FUEL_TRIM:
        read_fuel_trim(data);
        ESP_LOGI("SM", "TRIM: %f", data->parameters.fuel_trim);
        break;
    case STATE_BAROP:
        read_atmosphere_pressure(data);
        ESP_LOGI("SM", "BAROP: %f", data->parameters.barop);
        break;
    case STATE_INPUT_SWITCHES:
        ESP_LOGI("SM", "IN");
        read_input_switches(data);
        break;
    case STATE_INOUT_SWITCHES:
        ESP_LOGI("SM", "IO");
        read_inout_switches(data);
        break;
    case STATE_ACTIVE_CODE_ONE:
        ESP_LOGI("SM", "A1");
        read_active_trouble_code_one(data);
        break;
    case STATE_ACTIVE_CODE_TWO:
        ESP_LOGI("SM", "A2");
        read_active_trouble_code_two(data);
        break;
    case STATE_ACTIVE_CODE_THREE:
        ESP_LOGI("SM", "A3");
        read_active_trouble_code_three(data);
        break;
    case STATE_STORED_CODE_ONE:
        ESP_LOGI("SM", "S1");
        read_stored_trouble_code_one(data);
        break;
    case STATE_STORED_CODE_TWO:
        ESP_LOGI("SM", "S2");
        read_stored_trouble_code_two(data);
        break;
    case STATE_STORED_CODE_THREE:
        ESP_LOGI("SM", "S3");
        read_stored_trouble_code_three(data);
        break;
    }
    xQueueSend(lcd_queue, (void *)data, portMAX_DELAY);
}

static inline void process_event(struct state_machine_data *data, state_event_e event)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(state_transitions); i++)
    {
        if ((data->state == state_transitions[i].from) && (event == state_transitions[i].event))
        {
            state_enter(data, state_transitions[i].from, event, state_transitions[i].to);
        }
    }
}

static inline state_event_e process_input(void)
{
    state_event_e recieved_event;
    if (xQueueReceive(event_queue, (void *)&recieved_event, portMAX_DELAY) == pdPASS)
    {
        return recieved_event;
    }
    return STATE_EVENT_NONE;
}

void state_machine_task(void *parameters)
{
    static struct state_machine_data data;
    uart_init();
    ESP_LOGI("SM", "uart initialized");
    state_machine_init(&data);
    for (;;)
    {
        state_event_e next_event = process_input();
        process_event(&data, next_event);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}