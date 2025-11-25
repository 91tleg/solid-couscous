#ifndef ASSERT_HANDLER_H

#include <stdint.h>

#define ASSERT(expression)                                                    \
    do {                                                                      \
        if (!(expression))                                                    \
        {                                                                     \
            uint32_t pc = (uint32_t)__builtin_return_address(0);              \
            assert_handler(pc);                                               \
        }                                                                     \
    } while (0)

void assert_handler(uint32_t program_counter);

#endif // ASSERT_HANDLER_H