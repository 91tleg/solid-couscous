#include "state_machine.h"

struct state_machine_data
{
    state_e state;
    struct ecu_params parameters;
    struct input_switches status0;
    struct io_switches status1;
    struct trouble_code_one status2;
    struct trouble_code_two status3;
    struct trouble_code_three status4;
};

struct state_transition
{
    state_e from;
    state_event_e event; // Event that triggers the transition
    state_e to;
};

static const struct state_transition state_transitions[] = {
    {STATE_BATTERY_V, STATE_EVENT_BUTTON_PRESS, STATE_VEHICLE_SPEED},
    {STATE_VEHICLE_SPEED, STATE_EVENT_BUTTON_PRESS, STATE_ENGINE_SPEED}
};

void state_machine_init(struct state_machine_data *data)
{
    data->state = STATE_BATTERY_V;
    memset(&data->parameters, 0, sizeof(data->parameters));
    memset(&data->status0, 0, sizeof(data->status0));
    memset(&data->status1, 0, sizeof(data->status1));
    memset(&data->status2, 0, sizeof(data->status2));
    memset(&data->status3, 0, sizeof(data->status3));
    memset(&data->status4, 0, sizeof(data->status4));
}

void state_enter(struct state_machine_data *data, state_e from, state_event_e next_event, state_e to)
{
    data->state = to;
}

void process_event(struct state_machine_data *data, state_event_e next_event)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(state_transitions); i++)
    {
        if (data->state == state_transitions[i].from && next_event == state_transitions[i].event)
        {
            state_enter(data, state_transitions[i].from, next_event, state_transitions[i].to);
        }
    }
}

void state_machine_run(struct state_machine_data *data, state_event_e event) {
    process_event(data, event);  // Process the event and transition the state
}
