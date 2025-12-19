#include <unity.h>

void test_decode_battery_voltage(void);
void test_decode_vehicle_speed(void);
void test_decode_engine_speed(void);
void test_decode_coolant_temp_high(void);
void test_decode_coolant_temp_low(void);
void test_decode_coolant_temp_normal(void);
void test_decode_airflow(void);
void test_decode_throttle_percentage(void);
void test_decode_manifold_pressure(void);
void test_decode_boost_control_duty_cycle(void);
void test_decode_ignition_timing(void);
void test_decode_engine_load(void);
void test_decode_injector_pulse_width(void);
void test_decode_iacv_duty_cycle(void);
void test_decode_o2_signal(void);
void test_decode_timing_correction(void);
void test_decode_fuel_trim(void);
void test_decode_atmosphere_pressure(void);

void test_decode_input_switches(void);
void test_decode_io_switches(void);
void test_decode_trouble_code_one(void);
void test_decode_trouble_code_two(void);
void test_decode_trouble_code_three(void);

void setUp(void) {}

void tearDown(void) {}

#if defined(ESP_PLATFORM)
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void test_task(void *pvParameters)
{
    UNITY_BEGIN();

    RUN_TEST(test_decode_battery_voltage);
    RUN_TEST(test_decode_vehicle_speed);
    RUN_TEST(test_decode_engine_speed); 
    RUN_TEST(test_decode_coolant_temp_high);
    RUN_TEST(test_decode_coolant_temp_low);
    RUN_TEST(test_decode_coolant_temp_normal);
    RUN_TEST(test_decode_airflow);
    RUN_TEST(test_decode_throttle_percentage);
    RUN_TEST(test_decode_manifold_pressure);
    RUN_TEST(test_decode_boost_control_duty_cycle);
    RUN_TEST(test_decode_ignition_timing);
    RUN_TEST(test_decode_engine_load);
    RUN_TEST(test_decode_injector_pulse_width);
    RUN_TEST(test_decode_iacv_duty_cycle);
    RUN_TEST(test_decode_o2_signal);
    RUN_TEST(test_decode_timing_correction);
    RUN_TEST(test_decode_fuel_trim);
    RUN_TEST(test_decode_atmosphere_pressure);

    RUN_TEST(test_decode_input_switches);
    RUN_TEST(test_decode_io_switches);
    RUN_TEST(test_decode_trouble_code_one);
    RUN_TEST(test_decode_trouble_code_two);
    RUN_TEST(test_decode_trouble_code_three);

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

    RUN_TEST(test_decode_battery_voltage);
    RUN_TEST(test_decode_vehicle_speed);
    RUN_TEST(test_decode_engine_speed); 
    RUN_TEST(test_decode_coolant_temp_high);
    RUN_TEST(test_decode_coolant_temp_low);
    RUN_TEST(test_decode_coolant_temp_normal);
    RUN_TEST(test_decode_airflow);
    RUN_TEST(test_decode_throttle_percentage);
    RUN_TEST(test_decode_manifold_pressure);
    RUN_TEST(test_decode_boost_control_duty_cycle);
    RUN_TEST(test_decode_ignition_timing);
    RUN_TEST(test_decode_engine_load);
    RUN_TEST(test_decode_injector_pulse_width);
    RUN_TEST(test_decode_iacv_duty_cycle);
    RUN_TEST(test_decode_o2_signal);
    RUN_TEST(test_decode_timing_correction);
    RUN_TEST(test_decode_fuel_trim);
    RUN_TEST(test_decode_atmosphere_pressure);

    RUN_TEST(test_decode_input_switches);
    RUN_TEST(test_decode_io_switches);
    RUN_TEST(test_decode_trouble_code_one);
    RUN_TEST(test_decode_trouble_code_two);
    RUN_TEST(test_decode_trouble_code_three);

    return UNITY_END();
}

#endif