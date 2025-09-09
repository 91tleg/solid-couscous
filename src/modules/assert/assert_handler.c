#include "assert_handler.h"
#include "drivers/led/led.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#define TAG  "ASSERT"

static void assert_trace(uint32_t program_counter)
{
    ESP_LOGE(TAG, "ASSERT FAILED AT PC 0x%08" PRIx32, program_counter);
}

static void assert_blink_led(void)
{
    led_init();
    for(;;)
    {
        led_on();
        vTaskDelay(pdMS_TO_TICKS(300));
        led_off();
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void assert_handler(uint32_t program_counter)
{
    assert_trace(program_counter);
    assert_blink_led();
}