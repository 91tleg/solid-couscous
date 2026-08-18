#pragma once
#include <stdint.h>
#include "fsm_states.h"

struct romid_ctx {
    uint8_t cmd_index;   // 0 or 1
};

struct read_ctx {
    fsm_state_e state;
    uint16_t addr;
};
