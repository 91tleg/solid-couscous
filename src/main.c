#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "core/log/log.h"
#include "modules/button/button_task.h"
#include "modules/state_machine/state_machine_task.h"
#include "modules/lcd/lcd_task.h"

#define TAG  "Main"

#ifndef UNIT_TEST
void app_main(void)
{
    log_level_set(LOG_LEVEL_NONE);
    LOGI(TAG, "Starting application");

    xTaskCreate(
        button_task,
        "ButtonTask",
        2048,
        NULL,
        3,
        NULL
    );

    xTaskCreate(
        state_machine_task,
        "State Machine Task",
        4096,
        NULL,
        2,
        NULL
    );

    xTaskCreate(
        lcd_task,
        "Lcd Task",
        4096,
        NULL,
        1,
        NULL
    );
}
#endif  // UNIT_TEST