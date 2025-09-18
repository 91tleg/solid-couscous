#ifndef ECU_SIM_PINS_H
#define ECU_SIM_PINS_H 

#include <driver/gpio.h>

#define BTN_PIN             (GPIO_NUM_27)
#define I2C_MASTER_SCL_IO   (GPIO_NUM_22)
#define I2C_MASTER_SDA_IO   (GPIO_NUM_21)
#define LED_PIN             (GPIO_NUM_2)
#define TX_PIN              (GPIO_NUM_1)
#define RX_PIN              (GPIO_NUM_3)

#endif // ECU_SIM_PINS_H