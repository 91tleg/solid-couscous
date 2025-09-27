#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "drivers/drivers_init.h"
#include "core/log/log.h"
#include "modules/button/button_task.h"
#include "modules/state_machine/state_machine_task.h"
#include "modules/lcd/lcd_task.h"

#define TAG  "Main"

#ifndef UNIT_TEST
void app_main(void)
{
    log_init();
    LOGI(TAG, "Starting application");

    drivers_init();

    button_task_init();
    state_machine_task_init();
    lcd_task_init();

    LOGI(TAG, "Startup complete. Free heap: %u bytes", xPortGetFreeHeapSize());
}
#endif  // UNIT_TEST