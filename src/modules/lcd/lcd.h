#ifndef LCD_H
#define LCD_H

#include "state_defs.h"

/**
 * @brief Prints the current state to the lcd display.
 *
 * Updates the lcd contents based on the provided state machine data and current state.
 *
 * @param data Pointer to the state_machine_data struct containing current state info.
 */
void lcd_print_state(struct state_machine_data *data);

#endif // LCD_H