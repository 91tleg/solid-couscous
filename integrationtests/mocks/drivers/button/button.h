#ifndef MOCK_BUTTON_DRIVER_H
#define MOCK_BUTTON_DRIVER_H

#include "state_defs.h"

static inline void button_driver_init(void) {}

static int next_button_state = 0;

static inline void mock_set_button_state(int state)
{
    next_button_state = state;
}

static inline int button_driver_read(void)
{
    int state = next_button_state;
    next_button_state = 0;
    return state;
}

#endif // MOCK_BUTTON_DRIVER_H