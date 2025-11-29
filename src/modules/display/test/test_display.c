#include <unity.h>
#include <stdio.h>
#include "fsm_data.h"
#include "src/display.h"

void setUp(void) {}
void tearDown(void) {}

static char line0[21];
static char line1[21];
static int current_row = 0;

void hd44780_set_cursor(int col, int row)
{
    current_row = row;
}

void hd44780_print(const char *str)
{
    if (current_row == 0)
    {
        snprintf(line0, sizeof(line0), "%-20s", str);
    } 
    else if (current_row == 1)
    {
        snprintf(line1, sizeof(line1), "%-20s", str);
    }
}

void test_empty_romid_display(void)
{
    struct fsm_data data = {0};
    data.state = STATE_ROMID;

    display_print_state(&data);

    TEST_ASSERT_EQUAL_STRING("ROM ID: --.--.--   ", line0);
    TEST_ASSERT_EQUAL_STRING("                    ", line1);
}

void test_romid_display(void)
{
    struct fsm_data data = {0};
    data.state = STATE_ROMID;
    data.romid[0] = 0x12;
    data.romid[1] = 0x34;
    data.romid[2] = 0x56;

    display_print_state(&data);

    TEST_ASSERT_EQUAL_STRING("ROM ID: 12.34.56    ", line0);
    TEST_ASSERT_EQUAL_STRING("                    ", line1);
}

void test_vehicle_speed_display(void)
{
    struct fsm_data data = {0};
    data.state = STATE_VEHICLE_SPEED;
    data.decoded_data.u8 = 55;

    lcd_print_state(&data);

    TEST_ASSERT_EQUAL_STRING("VSPD: 55mph         ", line0);
}