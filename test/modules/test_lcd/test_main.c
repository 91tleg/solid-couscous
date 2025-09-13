#include <unity.h>


void test_romid_display(void);
void test_vehicle_speed_display(void);


#if defined(ESP_PLATFORM)

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void test_task(void *pvParameters)
{
    UNITY_BEGIN();

    RUN_TEST(test_romid_display);
    RUN_TEST(test_vehicle_speed_display);

    UNITY_END();
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(&test_task, "test_task", 4096, NULL, 5, NULL);
}

#else

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_romid_display);
    RUN_TEST(test_vehicle_speed_display);

    return UNITY_END();
}

#endif