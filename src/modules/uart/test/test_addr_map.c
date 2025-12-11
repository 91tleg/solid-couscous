#include <unity.h>
#include "../src/addr_map.h"

void test_lookup_addr(void)
{
    uint16_t addr;
    addr = lookup_addr(STATE_BATTERY_V);
    TEST_ASSERT_EQUAL_UINT16(0X1404, addr);

    addr = lookup_addr(STATE_VEHICLE_SPEED);
    TEST_ASSERT_EQUAL_UINT16(0X154B, addr);

    addr = lookup_addr((fsm_state_e)100);
    TEST_ASSERT_EQUAL_UINT16(0, addr);
}
