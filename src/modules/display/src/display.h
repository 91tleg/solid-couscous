#ifndef LCD_H
#define LCD_H

struct fsm_data;

/**
 * @brief Prints the current state to the display.
 *
 * Updates the display based on the provided current state and its data.
 *
 * @param data Pointer to the fsm_data struct containing current state info.
 */
void display_print_state(struct fsm_data *data);

#endif // LCD_H