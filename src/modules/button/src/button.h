#ifndef BUTTON_H
#define BUTTON_H

#include "state_defs.h"

/**
 * @brief Reads the button state and returns a state event.
 * 
 * Detects short press, double press, and long press.
 * 
 * @return state_event_e The event detected.
 */
state_event_e read_state_event(void);

#endif // BUTTON_H