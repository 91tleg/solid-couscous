#ifndef GET_PARAMETERS_H
#define GET_PARAMETERS_H 

#include <stdint.h>
#include "modules/state_machine/state_machine.h"

void get_romid(uint8_t *romid_out);
union param_type get_param(state_e state);
void reset_ctx_sent_flag(state_e state);

#endif // GET_PARAMETERS_H