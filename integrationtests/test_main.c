#include <stdio.h>
#include <unity.h>

void test_button_state_machine(void);

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_button_state_machine);
    return UNITY_END();
}