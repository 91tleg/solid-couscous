#include "assert_handler.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN (GPIO_NUM_2)

static void led_pin_init(void)
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

static void assert_trace(uint32_t program_counter)
{
    ESP_LOGI("ERROR", "ASSERT FAILED AT PC 0x%08" PRIx32, program_counter);
}

static void assert_blink_led(void)
{
    led_pin_init();
    for(;;)
    {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(300));
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void assert_handler(uint32_t program_counter)
{
    assert_trace(program_counter);
    assert_blink_led();
}