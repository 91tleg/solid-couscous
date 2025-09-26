#include <stdio.h>

void test_button_state_machine(void);

int main(void)
{
    printf("[TEST] Running integration test\n");
    test_button_state_machine();
    printf("[TEST] All tests passed\n");
    return 0;
}
