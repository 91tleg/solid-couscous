#include <unity.h>

void test_single_press(void);
void test_double_press(void);
void test_long_press(void);

#if defined(ESP_PLATFORM)

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void test_task(void *pvParameters)
{
    UNITY_BEGIN();

    RUN_TEST(test_single_press);
    RUN_TEST(test_long_press);

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

    RUN_TEST(test_single_press);
    RUN_TEST(test_long_press);

    return UNITY_END();
}

#endif