#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <driver/i2c_master.h>
#include <esp_err.h>
#include <freertos/FreeRTOS.h>

esp_err_t i2c_init(void);
i2c_master_dev_handle_t i2c_master_get_device(void);
esp_err_t i2c_master_write(i2c_master_dev_handle_t handle, uint8_t *data, size_t len, TickType_t timeout);

#endif // I2C_H