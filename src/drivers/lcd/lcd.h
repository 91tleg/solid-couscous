#ifndef DRIVERS_LCD_H
#define DRIVERS_LCD_H

#include <stdint.h>

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(char *str);

#endif // DRIVERS_LCD_H