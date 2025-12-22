#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stddef.h>
#include <esp_err.h>

typedef struct i2c_master_bus_t *i2c_master_bus_handle_t;
typedef struct i2c_master_dev_t *i2c_master_dev_handle_t;

esp_err_t i2c_driver_init(i2c_master_bus_handle_t *bus,
                          i2c_master_dev_handle_t *dev);

esp_err_t i2c_master_write(uint8_t *data, size_t len, int timeout);

#endif // I2C_H