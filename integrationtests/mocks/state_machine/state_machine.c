#include "state_machine.h"
#include <string.h>

void state_machine_data_init(struct state_machine_data *data)
{
    memset(data, 0, sizeof(*data));
    data->state = STATE_ROMID;
}

void process_event(struct state_machine_data *data, state_event_e event)
{
    if (event == STATE_EVENT_BUTTON_PRESS)
    {
        data->state = STATE_BATTERY_V;
    }
}