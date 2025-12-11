#include <unity.h>
#include "../src/ssm1.h"

void test_get_romid_command(void)
{
    struct romid_ctx ctx = {0};
    uint8_t cmd[4] = {0};

    ctx.cmd_index = 0;
    uint8_t expected[4] = {0x78, 0xFF, 0xFE, 0x00};
    ssm1_get_romid_command(&ctx, cmd);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, cmd, 4);

    *expected = 0x00;
    *(expected + 1) = *(expected + 2) = 0xFF;
    ssm1_get_romid_command(&ctx, cmd);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, cmd, 4);

    *expected = 0x78;
    *(expected + 1) = 0xFF;
    *(expected + 2) = 0xFE;
    ssm1_get_romid_command(&ctx, cmd);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, cmd, 4);
}

void test_get_read_command(void)
{
    struct read_ctx ctx = {0};
    uint8_t cmd[4] = {0};

    ctx.addr = 0x1010;
    uint8_t expected[4] = {0x78, 0x10, 0x10, 0x00};
    ssm1_get_read_command(&ctx, cmd);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, cmd, 4);

    ctx.addr = 0x1234;
    *(expected + 1) = 0x12;
    *(expected + 2) = 0x34;
    ssm1_get_read_command(&ctx, cmd);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, cmd, 4);
}

void test_get_clear_command(void)
{
    struct read_ctx ctx = {0};
    uint8_t cmd[4] = {0};

    ctx.addr = 0x1010;
    uint8_t expected[4] = {0xAA, 0x10, 0x10, 0x00};
    ssm1_get_clear_command(&ctx, cmd);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, cmd, 4);

    ctx.addr = 0x1234;
    *(expected + 1) = 0x12;
    *(expected + 2) = 0x34;
    ssm1_get_clear_command(&ctx, cmd);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, cmd, 4);
}
