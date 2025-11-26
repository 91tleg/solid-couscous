#ifndef ASSERT_HANDLER_H
#define ASSERT_HANDLER_H

#include <stdint.h>

#if !defined(NDEBUG)
#define ASSERT(expr)                                             \
    do {                                                         \
        if (!(expr))                                             \
        {                                                        \
            uint32_t pc = (uint32_t)__builtin_return_address(0); \
            assert_handler(pc, __FILE__, __LINE__);              \
        }                                                        \
    } while (0)
#else
#define ASSERT(expr) ((void)0)
#endif // NDEBUG

void assert_handler(uint32_t pc, const char *file, int line);

#endif // ASSERT_HANDLER_H