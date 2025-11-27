#include "button.h"
#include "board.h"
#include "timer.h"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

static void IRAM_ATTR button_isr_handler(void *arg)
{
    QueueHandle_t isr_queue = (QueueHandle_t)arg;

    struct button_isr evt = {
        .level = button_driver_read(),
        .timestamp_us = timer_get_time_us(),
    };

    xQueueSendFromISR(isr_queue, &evt, NULL);
}

void button_driver_init(QueueHandle_t isr_queue)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BTN_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };

    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BTN_PIN, button_isr_handler, (void *)isr_queue);
}

int button_driver_read(void)
{
    return gpio_get_level(BTN_PIN);
}
