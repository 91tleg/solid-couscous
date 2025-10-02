#ifndef MOCK_STATE_MACHINE_H
#define MOCK_STATE_MACHINE_H

#include "state_defs.h"

#ifdef __cplusplus
extern "C" 
{
#endif

void state_machine_data_init(struct state_machine_data *data);
void process_event(struct state_machine_data *data, state_event_e event);

#ifdef __cplusplus
}
#endif

#endif // MOCK_STATE_MACHINE_H