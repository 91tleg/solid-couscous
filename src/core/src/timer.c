#include "timer.h"
#include <esp_timer.h>

int64_t timer_get_time_us(void) 
{
    return esp_timer_get_time();
}