#ifndef LCD_TASK_H
#define LCD_TASK_H

/**
 * @brief Initializes the lcd task.
 */
void lcd_task_init(void);

/**
 * @brief Lcd display task for handling screen updates.
 * 
 * Manages the lcd display, updating its contents based on system state.
 * 
 * @param parameters Pointer to task-specific parameters (unused).
 */
void lcd_task(void *parameters);

#endif // LCD_TASK_H