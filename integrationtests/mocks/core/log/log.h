#ifndef MOCK_LOG_H
#define MOCK_LOG_H

#include "defines.h"

typedef enum
{
    LOG_LEVEL_NONE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
} log_level_e;

static inline void log_level_set(log_level_e level) {}

static inline void log_message(log_level_e level, const char *tag, const char *fmt, ...)
{
    UNUSED(level);
    UNUSED(tag);
    UNUSED(fmt);
}

#define LOGI(tag, fmt, ...) log_message(LOG_LEVEL_INFO, tag, fmt, ##__VA_ARGS__)
#define LOGW(tag, fmt, ...) log_message(LOG_LEVEL_WARN, tag, fmt, ##__VA_ARGS__)
#define LOGE(tag, fmt, ...) log_message(LOG_LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)

#endif // MOCK_LOG_H