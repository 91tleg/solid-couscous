#include "led.h"
#include <driver/gpio.h>

#define LED_PIN     (GPIO_NUM_2)
#define LED_ON      (1)
#define LED_OFF     (0)

void led_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << LED_PIN,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

void led_on(void)
{
    gpio_set_level(LED_PIN, LED_ON);
}

void led_off(void)
{
    gpio_set_level(LED_PIN, LED_OFF);
}