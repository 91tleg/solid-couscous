#ifndef FSM_DATA_H
#define FSM_DATA_H

#include "fsm_states.h"
#include "parameters.h"
#include <stdint.h>

struct fsm_data
{
    fsm_state_e state;
    union param_type decoded_data;
    uint8_t romid[3];
};

#endif // FSM_DATA_H