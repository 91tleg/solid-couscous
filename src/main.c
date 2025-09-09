#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <esp_log.h>
#include "modules/button/button.h"
#include "modules/state_machine/state_machine.h"
#include "modules/lcd/lcd.h"

#define TAG  "MAIN"

#ifndef UNIT_TEST
void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_NONE);
    ESP_LOGI(TAG, "Starting application");

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
#endif  // UNIT_TEST