#include "lcd_task.h"
#include "drivers/lcd/lcd.h"
#include "lcd.h"
#include "state_defs.h"
#include "modules/state_machine/state_machine_task.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

void lcd_task(void *parameters)
{
    lcd_init();
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