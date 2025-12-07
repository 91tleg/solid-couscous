#include <unity.h>
#include "../src/fsm.h"

void setUp() {}
void tearDown() {}

void test_fsm_process_event(void)
{
    button_event_e event;
    fsm_state_e state = STATE_ROMID;

    event = BUTTON_EVENT_NONE;
    fsm_process_event(&state, event);
    TEST_ASSERT_EQUAL(STATE_ROMID, state);

    event = BUTTON_EVENT_SINGLE_PRESS;
    fsm_process_event(&state, event);
    TEST_ASSERT_EQUAL(STATE_BATTERY_V, state);

    event = BUTTON_EVENT_SINGLE_PRESS;
    fsm_process_event(&state, event);
    TEST_ASSERT_EQUAL(STATE_VEHICLE_SPEED, state);

    event = BUTTON_EVENT_LONG_PRESS;
    fsm_process_event(&state, event);
    TEST_ASSERT_EQUAL(STATE_ROMID, state);

    event = BUTTON_EVENT_LONG_PRESS;
    fsm_process_event(&state, event);
    TEST_ASSERT_EQUAL(STATE_ROMID, state);
}