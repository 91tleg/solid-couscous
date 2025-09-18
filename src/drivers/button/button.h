#ifndef DRIVERS_BUTTON_H
#define DRIVERS_BUTTON_H

/**
 * @brief Initialize the button gpio.
 * 
 * This function should be called once before reading the button state.
 */
void button_driver_init(void);

/**
 * @brief Read the button gpio state.
 * @return 0 if button is pressed, else 1.
 * 
 * This function returns the current raw state of the button gpio.
 */
int button_driver_read(void);

#endif // DRIVERS_BUTTON_H