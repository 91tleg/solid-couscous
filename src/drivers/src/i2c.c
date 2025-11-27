#include "i2c.h"
#include "log.h"
#include "board.h"
#include <driver/i2c_master.h>

#define TAG                 "I2c"
#define I2C_MASTER_FREQ_HZ  (100000U)
#define I2C_SLAVE_ADDR      (0x27U)

static i2c_master_dev_handle_t dev_handle = NULL;

esp_err_t i2c_driver_init(i2c_master_bus_handle_t *bus, 
                          i2c_master_dev_handle_t *dev)
{
    esp_err_t err = ESP_OK;
    i2c_master_bus_config_t i2c_mst_conf = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_NUM,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };
    err = i2c_new_master_bus(&i2c_mst_conf, bus);
    if (err != ESP_OK) { goto error; }

    i2c_device_config_t dev_conf = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_SLAVE_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ
    };
    err = i2c_master_bus_add_device(*bus, &dev_conf, dev);
    if (err != ESP_OK) { goto error; }

    dev_handle = *dev;

    LOGI(TAG, "I2C device added at address 0x%02X", I2C_SLAVE_ADDR);
    return err;

error:
    LOGE(TAG, "Failed to config i2c: %s", esp_err_to_name(err));
    return err;
}

esp_err_t i2c_master_write(uint8_t *data, size_t len, int timeout)
{
    esp_err_t err = i2c_master_transmit(dev_handle, data, len, timeout);
    LOGI(TAG, "TX %zu bytes to device (err=%s)", len, esp_err_to_name(err));
    return err;
}