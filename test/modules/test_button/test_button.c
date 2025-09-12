#include <unity.h>
#include "modules/button/button.h"

#define DEBOUNCE_TIME_US    (1000UL)
#define SHORT_PRESS_TIME_US (300000UL)
#define LONG_PRESS_TIME_US  (800000UL)

static int mock_button_state = 1;
static uint64_t mock_time = 0;

void log_message(int level, const char *tag, const char *fmt, ...) {}

int button_driver_read(void)
{
    return mock_button_state;
}

int64_t timer_get_time_us(void)
{
    return mock_time;
}


static void advance_time_us(uint64_t us)
{
    mock_time += us;
}

static void set_button_state(int state)
{
    mock_button_state = state;
}

static state_event_e wait_for_event(void)
{
    state_event_e event;
    do {
        event = read_state_event();
    } while(event == STATE_EVENT_NONE);
    return event;
}

static state_event_e poll_event(uint64_t advance_us)
{
    state_event_e event = STATE_EVENT_NONE;
    for (int i = 0; i < 100; i++)
    {
        advance_time_us(advance_us);
        event = read_state_event();
        if (event != STATE_EVENT_NONE)
        {
            break;
        }
    }
    return event;
}

void setUp(void)
{
    mock_button_state = 1; // Released
    mock_time = 0;
}

void tearDown(void) {}

void test_single_press(void)
{
    set_button_state(0);
    poll_event(DEBOUNCE_TIME_US + 1);

    set_button_state(1);
    TEST_ASSERT_EQUAL(STATE_EVENT_BUTTON_PRESS, poll_event(SHORT_PRESS_TIME_US + DEBOUNCE_TIME_US + 1));
}

void test_long_press(void)
{
    set_button_state(0);
    TEST_ASSERT_EQUAL(STATE_EVENT_BUTTON_LONG_PRESS, poll_event(LONG_PRESS_TIME_US + DEBOUNCE_TIME_US + 1));

    set_button_state(1);
    poll_event(DEBOUNCE_TIME_US + 1);
}

void test_double_press(void)
{
    // First press
    set_button_state(0);
    poll_event(DEBOUNCE_TIME_US + 1); // debounce
    set_button_state(1);
    poll_event(DEBOUNCE_TIME_US + 1); // release

    // Second press
    set_button_state(0);
    poll_event(DEBOUNCE_TIME_US + 1); // debounce
    set_button_state(1);
    poll_event(DEBOUNCE_TIME_US + 1); // release

    TEST_ASSERT_EQUAL(STATE_EVENT_BUTTON_DOUBLE_PRESS, poll_event(SHORT_PRESS_TIME_US + 1));
}