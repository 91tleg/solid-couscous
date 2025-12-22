#ifndef FSM_DATA_H
#define FSM_DATA_H

#include "fsm_states.h"
#include "parameters.h"
#include <stdint.h>

struct fsm_data
{
    fsm_state_e state;
    struct ecu_params params;
    struct input_switches in_sw;
    struct io_switches io_sw;
    struct trouble_code_one tc1;
    struct trouble_code_two tc2;
    struct trouble_code_three tc3;
};

#endif // FSM_DATA_H