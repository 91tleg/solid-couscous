#include <unity.h>
#include <stdio.h>
#include "state_defs.h"
#include "modules/lcd/lcd.h"

void setUp(void) {}
void tearDown(void) {}

void log_message(int level, const char *tag, const char *fmt, ...) {}

static char last_line0[21];
static char last_line1[21];
static int current_row;

void lcd_set_cursor(int col, int row)
{
    current_row = row;
}

void lcd_print(const char *str)
{
    if (current_row == 0)
    {
        snprintf(last_line0, sizeof(last_line0), "%-20s", str);
    } 
    else if (current_row == 1)
    {
        snprintf(last_line1, sizeof(last_line1), "%-20s", str);
    }
}

void test_romid_display(void)
{
    struct state_machine_data data = {0};
    data.state = STATE_ROMID;
    data.parameters.romid[0] = 0x12;
    data.parameters.romid[1] = 0x34;
    data.parameters.romid[2] = 0x56;

    lcd_print_state(&data);

    TEST_ASSERT_EQUAL_STRING("ROM ID: 12.34.56    ", last_line0);
    TEST_ASSERT_EQUAL_STRING("                    ", last_line1);
}

void test_vehicle_speed_display(void)
{
    struct state_machine_data data = {0};
    data.state = STATE_VEHICLE_SPEED;
    data.parameters.vehicle_speed = 55;

    lcd_print_state(&data);

    TEST_ASSERT_EQUAL_STRING("VSPD: 55mph         ", last_line0);
}