#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "button.h"
#include "state_machine.h"
#include "lcd.h"

void app_main()
{
    ESP_LOGI("MAIN", "Starting application.....");

    event_queue = xQueueCreate(5, sizeof(state_event_e));
    lcd_queue = xQueueCreate(5, sizeof(struct state_machine_data));

    xTaskCreate(
        button_task,
        "ButtonTask",
        2048,
        NULL,
        3,
        NULL);

    xTaskCreate(
        state_machine_task,
        "State Machine Task",
        2048,
        NULL,
        2,
        NULL);

    xTaskCreate(
        lcd_task,
        "Lcd Task",
        2048,
        NULL,
        1,
        NULL);
}