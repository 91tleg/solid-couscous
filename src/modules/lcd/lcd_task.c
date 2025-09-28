#include "lcd_task.h"
#include "drivers/lcd/lcd.h"
#include "lcd.h"
#include "core/log/log.h"
#include "core/assert/assert_handler.h"
#include "state_defs.h"
#include "modules/state_machine/state_machine_task.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#define TAG "LcdTask"

void lcd_task_init(void)
{
    BaseType_t res = xTaskCreate(
        lcd_task,
        "Lcd Task",
        4096,
        NULL,
        1,
        NULL
    );
    ASSERT(res == pdPASS);
    LOGI(TAG, "Lcd task started.");
}

void lcd_task(void *parameters)
{
    struct state_machine_data lcd_data;
    QueueHandle_t lcd_queue = state_machine_get_lcd_queue();
    for (;;)
    {
        if (xQueueReceive(lcd_queue, &lcd_data, portMAX_DELAY) == pdTRUE)
        {
            lcd_print_state(&lcd_data);
        }
        vTaskDelay(pdMS_TO_TICKS(90));
    }
}