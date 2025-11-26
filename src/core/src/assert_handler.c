#include "assert_handler.h"
#include "led.h"
#include "log.h"
#include <inttypes.h>

#define TAG  "Assert"

static void assert_blink_led(void)
{
    for (;;)
    {
        led_on();
        for (volatile int i = 0; i < 1000000; ++i);
        led_off();
        for (volatile int i = 0; i < 1000000; ++i);
    }
}

void assert_handler(uint32_t pc, const char *file, int line)
{
    
    LOGE(TAG, "Assert failed at %s:%d, PC=0x%08" PRIX32,
         file, line, pc);
    assert_blink_led();
}
