#ifndef MOCK_STATE_MACHINE_TASK_H
#define MOCK_STATE_MACHINE_TASK_H

#include "state_defs.h"

void state_machine_data_init(struct state_machine_data *data);
void process_event(struct state_machine_data *data, state_event_e event);

#endif // MOCK_STATE_MACHINE_TASK_H