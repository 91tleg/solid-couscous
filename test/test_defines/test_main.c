#include <unity.h>
#include "defines.h"

#define TEST_BYTE_ONE (0xFF)
#define TEST_BYTE_TWO (0xF0)
#define TEST_BYTE_THREE (0x0F)
#define TEST_BYTE_FOUR (0x10)
#define TEST_BYTE_FIVE (0x01)

#define TEST_ADDR_ONE (0xFFFF)
#define TEST_ADDR_TWO (0x1010)
#define TEST_ADDR_THREE (0x0001)

void setUp(void)
{
}

void tearDown(void) 
{
}

void test_get_high_nibble(void)
{
    TEST_ASSERT_EQUAL(0xF0, GET_HIGH_NIB(TEST_BYTE_ONE));
    TEST_ASSERT_EQUAL(0xF0, GET_HIGH_NIB(TEST_BYTE_TWO));
    TEST_ASSERT_EQUAL(0x00, GET_HIGH_NIB(TEST_BYTE_THREE));
    TEST_ASSERT_EQUAL(0x10, GET_HIGH_NIB(TEST_BYTE_FOUR));
    TEST_ASSERT_EQUAL(0x00, GET_HIGH_NIB(TEST_BYTE_FIVE));
}

void test_get_low_nibble(void)
{
    TEST_ASSERT_EQUAL(0xF0, GET_LOW_NIB(TEST_BYTE_ONE));
    TEST_ASSERT_EQUAL(0x00, GET_LOW_NIB(TEST_BYTE_TWO));
    TEST_ASSERT_EQUAL(0xF0, GET_LOW_NIB(TEST_BYTE_THREE));
    TEST_ASSERT_EQUAL(0x00, GET_LOW_NIB(TEST_BYTE_FOUR));
    TEST_ASSERT_EQUAL(0x10, GET_LOW_NIB(TEST_BYTE_FIVE));
}

void test_get_msb(void)
{
    TEST_ASSERT_EQUAL(0xFF, GET_MSB(TEST_ADDR_ONE));
    TEST_ASSERT_EQUAL(0x10, GET_MSB(TEST_ADDR_TWO));
    TEST_ASSERT_EQUAL(0x00, GET_MSB(TEST_ADDR_THREE));
}

void test_get_lsb(void)
{
    TEST_ASSERT_EQUAL(0xFF, GET_LSB(TEST_ADDR_ONE));
    TEST_ASSERT_EQUAL(0x10, GET_LSB(TEST_ADDR_TWO));
    TEST_ASSERT_EQUAL(0x01, GET_LSB(TEST_ADDR_THREE));
}

void app_main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_get_high_nibble);
    RUN_TEST(test_get_low_nibble);
    RUN_TEST(test_get_msb);
    RUN_TEST(test_get_lsb);

    UNITY_END();
}