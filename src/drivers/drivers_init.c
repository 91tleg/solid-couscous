#include "drivers_init.h"
#include "button/button.h"
#include "i2c/i2c.h"
#include "lcd/lcd.h"
#include "led/led.h"
#include "uart/uart.h"

void drivers_init(void)
{
    button_driver_init();
    i2c_driver_init();
    lcd_driver_init();
    led_driver_init();
    uart_driver_init();
}