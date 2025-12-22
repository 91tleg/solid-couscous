#include <unity.h>
#include "../src/decode_parameters.h"

void test_decode_battery_voltage(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.f, decode_battery_voltage(0));
    TEST_ASSERT_EQUAL_FLOAT(20.4f, decode_battery_voltage(255));
}

void test_decode_vehicle_speed(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.f, decode_vehicle_speed(0));
    TEST_ASSERT_EQUAL_FLOAT(159.f, decode_vehicle_speed(255));
}

void test_decode_engine_speed(void)
{
    TEST_ASSERT_EQUAL_UINT16(0, decode_engine_speed(0));
    TEST_ASSERT_EQUAL_UINT16(6375, decode_engine_speed(255));
}

void test_decode_coolant_temp_high(void)
{
    TEST_ASSERT_EQUAL_INT16(331, decode_coolant_temp(5));
}

void test_decode_coolant_temp_low(void)
{
    TEST_ASSERT_EQUAL_INT16(-47, decode_coolant_temp(255));
}

void test_decode_coolant_temp_normal(void)
{
    TEST_ASSERT_EQUAL_INT16(243, decode_coolant_temp(15));
}

void test_decode_airflow(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.f, decode_airflow(0));
    TEST_ASSERT_EQUAL_FLOAT(5.1f, decode_airflow(255));
}

void test_decode_throttle_percentage(void)
{
    TEST_ASSERT_EQUAL_UINT8(0, decode_throttle_percentage(0));
    TEST_ASSERT_EQUAL_UINT8(99, decode_throttle_percentage(255));
}

void test_decode_manifold_pressure(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -41.7f, decode_manifold_pressure(0));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 18.02f, decode_manifold_pressure(255));
}

void test_decode_boost_control_duty_cycle(void)
{
    TEST_ASSERT_EQUAL_UINT8(0, decode_boost_control_duty_cycle(0));
    TEST_ASSERT_EQUAL_UINT8(99, decode_boost_control_duty_cycle(255));
}

void test_decode_ignition_timing(void)
{
    TEST_ASSERT_EQUAL_UINT8(0, decode_ignition_timing(0));
    TEST_ASSERT_EQUAL_UINT8(255, decode_ignition_timing(255));
}

void test_decode_engine_load(void)
{
    TEST_ASSERT_EQUAL_UINT8(0, decode_engine_load(0));
    TEST_ASSERT_EQUAL_UINT8(255, decode_engine_load(255));
}

void test_decode_injector_pulse_width(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.f, decode_injector_pulse_width(0));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 32.64f, decode_injector_pulse_width(255));
    
}

void test_decode_iacv_duty_cycle(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.f, decode_iacv_duty_cycle(0));
    TEST_ASSERT_EQUAL_FLOAT(127.5f, decode_iacv_duty_cycle(255));
}

void test_decode_o2_signal(void)
{
    TEST_ASSERT_EQUAL_FLOAT(0.f, decode_o2_signal(0));
    TEST_ASSERT_EQUAL_FLOAT(2.55f, decode_o2_signal(255));
}

void test_decode_timing_correction(void)
{
    TEST_ASSERT_EQUAL_UINT8(0, decode_timing_correction(0));
    TEST_ASSERT_EQUAL_UINT8(255, decode_timing_correction(255)); 
}

void test_decode_fuel_trim(void)
{
    TEST_ASSERT_EQUAL_FLOAT(-100.f, decode_fuel_trim(0));
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 99.2f, decode_fuel_trim(255));
}

void test_decode_atmosphere_pressure(void)
{
    TEST_ASSERT_EQUAL_FLOAT(500.f, decode_atmosphere_pressure(0));
    TEST_ASSERT_EQUAL_FLOAT(818.75f, decode_atmosphere_pressure(255));
}
