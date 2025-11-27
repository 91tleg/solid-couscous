#ifndef HD44780_H
#define HD44780_H

#include <stdint.h>

void hd44780_driver_init(void);
void hd44780_clear(void);
void hd44780_set_cursor(uint8_t col, uint8_t row);
void hd44780_print(char *str);

#endif // HD44780_H