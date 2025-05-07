#include <unity.h>
#include "decode_trouble_codes.h"
#include "parameters.h"

void setUp(void)
{
}

void tearDown(void) 
{
}

void test_decode_input_switches(void)
{
    struct input_switches test_data = {0};
    decode_input_switches(1, &test_data);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.ignition);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.auto_trans);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.test_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.read_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.neutral);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.park);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.california);

    decode_input_switches(2, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ignition);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.auto_trans);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.test_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.read_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.neutral);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.park);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.california);

    decode_input_switches(4, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ignition);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.auto_trans);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.test_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.read_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.neutral);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.park);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.california);

    decode_input_switches(8, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ignition);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.auto_trans);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.test_mode);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.read_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.neutral);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.park);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.california);

    decode_input_switches(16, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ignition);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.auto_trans);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.test_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.read_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.neutral);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.park);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.california);

    decode_input_switches(32, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ignition);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.auto_trans);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.test_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.read_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.neutral);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.park);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.california);

    decode_input_switches(64, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ignition);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.auto_trans);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.test_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.read_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.neutral);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.park);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.california);

    decode_input_switches(128, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ignition);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.auto_trans);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.test_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.read_mode);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.neutral);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.park);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.california);
}

void test_decode_io_switches(void)
{
    struct io_switches test_data = {0};
    decode_io_switches(1, &test_data);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_relay);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.rad_fan);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.fuel_pump);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.purge_valve);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.pinging);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.press_exch);

    decode_io_switches(2, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.ac_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_relay);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.rad_fan);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.fuel_pump);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.purge_valve);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.pinging);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.press_exch);

    decode_io_switches(4, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_sw);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.ac_relay);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.rad_fan);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.fuel_pump);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.purge_valve);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.pinging);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.press_exch);

    decode_io_switches(8, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_relay);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.rad_fan);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.fuel_pump);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.purge_valve);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.pinging);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.press_exch);

    decode_io_switches(16, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_relay);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.rad_fan);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.fuel_pump);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.purge_valve);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.pinging);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.press_exch);

    decode_io_switches(32, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_relay);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.rad_fan);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.fuel_pump);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.purge_valve);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.pinging);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.press_exch);

    decode_io_switches(64, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_relay);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.rad_fan);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.fuel_pump);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.purge_valve);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.pinging);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.press_exch);

    decode_io_switches(128, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.ac_relay);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.rad_fan);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.fuel_pump);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.purge_valve);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.pinging);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.press_exch);
}

void test_decode_trouble_code_one(void)
{
    struct trouble_code_one test_data = {0};
    decode_trouble_code_one(1, &test_data);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.crank);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.starter);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.cam);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_1);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_2);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_3);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_4);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_7);

    decode_trouble_code_one(2, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.crank);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.starter);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.cam);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_1);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_2);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_3);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_4);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_7);

    decode_trouble_code_one(4, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.crank);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.starter);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.cam);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_1);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_2);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_3);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_4);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_7);

    decode_trouble_code_one(8, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.crank);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.starter);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.cam);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.inj_1);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_2);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_3);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_4);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_7);

    decode_trouble_code_one(16, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.crank);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.starter);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.cam);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_1);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.inj_2);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_3);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_4);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_7);

    decode_trouble_code_one(32, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.crank);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.starter);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.cam);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_1);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_2);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.inj_3);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_4);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_7);

    decode_trouble_code_one(64, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.crank);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.starter);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.cam);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_1);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_2);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_3);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.inj_4);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_7);

    decode_trouble_code_one(128, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.crank);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.starter);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.cam);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_1);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_2);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_3);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.inj_4);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_7);
}

void test_decode_trouble_code_two(void)
{
    struct trouble_code_two test_data = {0};
    decode_trouble_code_two(0, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.temp);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.knock);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.maf);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.iacv);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.tps);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.oxygen);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.vss);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.purge);

    decode_trouble_code_two(255, &test_data);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.temp);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.knock);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.maf);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.iacv);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.tps);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.oxygen);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.vss);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.purge);

    decode_trouble_code_two(0, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.temp);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.knock);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.maf);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.iacv);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.tps);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.oxygen);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.vss);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.purge);
}

void test_decode_trouble_code_three(void)
{
    struct trouble_code_three test_data = {0};
    decode_trouble_code_three(0, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.fuel_trim);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_2);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.wgc);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.baro);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.wrong_maf);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.neutral_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.parking_sw);

    decode_trouble_code_three(255, &test_data);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.fuel_trim);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_2);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.wgc);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.baro);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.wrong_maf);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.neutral_sw);
    TEST_ASSERT_EQUAL_UINT8(1, test_data.parking_sw);

    decode_trouble_code_three(0, &test_data);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.fuel_trim);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.idle_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.unused_bit_2);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.wgc);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.baro);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.wrong_maf);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.neutral_sw);
    TEST_ASSERT_EQUAL_UINT8(0, test_data.parking_sw);
}

void app_main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_decode_input_switches);
    RUN_TEST(test_decode_io_switches);
    RUN_TEST(test_decode_trouble_code_one);
    RUN_TEST(test_decode_trouble_code_two);
    RUN_TEST(test_decode_trouble_code_three);

    UNITY_END();
}