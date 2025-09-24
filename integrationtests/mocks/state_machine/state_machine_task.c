#include "state_machine_task.h"

void state_machine_init(struct state_machine_data *data) {}
void process_event(struct state_machine_data *data, state_event_e event)
{
    if (event == STATE_EVENT_BUTTON_PRESS)
    {
        data->state = STATE_BATTERY_V;
    }
}

void uart_init(void) { }