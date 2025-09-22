#include <unity.h>

void test_send_clear_command(void);

#if defined(ESP_PLATFORM)

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void test_task(void *pvParameters)
{
    UNITY_BEGIN();

    RUN_TEST(test_send_clear_command);

    UNITY_END();

    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(&test_task, "test_task", 4096, NULL, 5, NULL);
}

#else

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_send_clear_command);

    return UNITY_END();
}

#endif