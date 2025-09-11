#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/**
 * @brief Get the current time in microseconds.
 *
 * @return int64_t Current time in microseconds.
 */
int64_t timer_get_time_us(void);

#endif // TIMER_H