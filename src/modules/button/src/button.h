#ifndef BUTTON_H
#define BUTTON_H

#include "button_event.h"
#include <stdint.h>

/**
 * @brief Reads the button state and time to detect button event.
 * @param level the level of button gpio.
 * @param timestamp the time when button isr kicked.
 * @return button_event_e The button event detected.
 */
button_event_e button_process_event(uint8_t level, uint64_t timestamp);

#endif // BUTTON_H