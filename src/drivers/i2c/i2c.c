#include "i2c.h"
#include <esp_log.h>

#define TAG                 "I2C"
#define I2C_MASTER_NUM      (I2C_NUM_0)
#define I2C_MASTER_SCL_IO   (GPIO_NUM_22)
#define I2C_MASTER_SDA_IO   (GPIO_NUM_21)
#define I2C_MASTER_FREQ_HZ  (100000U)
#define LCD_SLAVE_ADDR      (0x27U)

static i2c_master_dev_handle_t dev_handle = NULL;
static i2c_master_bus_handle_t bus_handle = NULL;

esp_err_t i2c_init(void)
{
    esp_err_t err = ESP_OK;
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_NUM,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };
    err = i2c_new_master_bus(&i2c_mst_config, &bus_handle);
    if (err != ESP_OK)
    {
        goto error;
    }

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = LCD_SLAVE_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ
    };
    err = i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle);
    if (err != ESP_OK)
    {
        goto error;
    }

    return err;

error:
    ESP_LOGE(TAG, "Failed to config i2c: %s", esp_err_to_name(err));
    return err;
}

i2c_master_dev_handle_t i2c_master_get_device(void)
{
    return dev_handle;
}

esp_err_t i2c_master_write(i2c_master_dev_handle_t handle, uint8_t *data, size_t len, TickType_t timeout)
{
    return i2c_master_transmit(handle, data, len, timeout);
}