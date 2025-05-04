#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "button.h"
#include "state_machine.h"
#include "lcd.h"

#ifndef UNIT_TEST
void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_NONE);
    ESP_LOGI("MAIN", "Starting application.....");

    event_queue = xQueueCreate(5, sizeof(state_event_e));
    ESP_LOGI("MAIN", "Event queue created...");

    lcd_queue = xQueueCreate(26, sizeof(struct state_machine_data));
    ESP_LOGI("MAIN", "LCD queue created...");

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
        4096,
        NULL,
        2,
        NULL);

    xTaskCreate(
        lcd_task,
        "Lcd Task",
        4096,
        NULL,
        1,
        NULL);
}
#endif