#ifndef ESP32DEV_PINS_H
#define ESP32DEV_PINS_H

#include <driver/gpio.h>

#define BTN_PIN             (GPIO_NUM_27)
#define I2C_MASTER_SCL_IO   (GPIO_NUM_22)
#define I2C_MASTER_SDA_IO   (GPIO_NUM_21)
#define LED_PIN             (GPIO_NUM_2)
#define TX_PIN              (GPIO_NUM_25)
#define RX_PIN              (GPIO_NUM_26)

#endif // ESP32DEV_PINS_H