#ifndef HD44780_H
#define HD44780_H

#include <stdint.h>

typedef struct i2c_master_dev_t *i2c_master_dev_handle_t;

void hd44780_driver_init(i2c_master_dev_handle_t dev);
void hd44780_clear(void);
void hd44780_set_cursor(uint8_t col, uint8_t row);
void hd44780_print(char *str);

#endif // HD44780_H