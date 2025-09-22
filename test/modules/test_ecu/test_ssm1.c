#include <unity.h>
#include <string.h>
#include "modules/ecu/ssm1.h"

static uint8_t bytes_recieved[4];
static uint8_t bytes_sent[4];
static struct read_ctx ctx;

void setUp(void)
{
    memset(bytes_recieved, 0, sizeof(bytes_recieved));
    memset(bytes_sent, 0, sizeof(bytes_sent));
    memset(&ctx, 0, sizeof(ctx));
}

void tearDown(void)
{
    memset(bytes_recieved, 0, sizeof(bytes_recieved));
    memset(bytes_sent, 0, sizeof(bytes_sent));
}


void send_bytes(const uint8_t *data, uint8_t len)
{
    for (int i = 0; i < len; i++)
    {
        bytes_recieved[i] = data[i];
    }
}

void read_bytes(uint8_t *buf, uint8_t max_len)
{
    for (int i = 0; i < max_len; i++)
    {
        buf[i] = bytes_sent[i];
    }
}

void test_send_clear_command(void)
{
    ctx.addr = 0x1010;
    uint8_t expected[4] = {0xAA, 0x10, 0x10, 0x00};
    send_clear_command(&ctx);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, bytes_recieved, 4);
}