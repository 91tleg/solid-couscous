#include <unity.h>

void test_get_romid_command(void);
void test_get_read_command(void);
void test_get_clear_command(void);
void test_lookup_addr(void);

void setUp(void) {}
void tearDown(void) {}

#if defined(ESP_PLATFORM)
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void test_task(void *pvParameters)
{
    UNITY_BEGIN();

    RUN_TEST(test_get_romid_command);
    RUN_TEST(test_get_read_command);
    RUN_TEST(test_get_clear_command);
    RUN_TEST(test_lookup_addr);

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

    RUN_TEST(test_get_romid_command);
    RUN_TEST(test_get_read_command);
    RUN_TEST(test_get_clear_command);
    RUN_TEST(test_lookup_addr);

    return UNITY_END();
}

#endif
