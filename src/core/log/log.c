#include "log.h"
#include <esp_log.h>
#include <stdarg.h>

static log_level_e current_log_level = LOG_LEVEL_INFO;

void log_level_set(log_level_e level)
{
    current_log_level = level;
}

void log_message(log_level_e level, const char *tag, const char *fmt, ...)
{
    if (level > current_log_level || level == LOG_LEVEL_NONE)
    {
        return; // skip logging
    }

    va_list args;
    va_start(args, fmt);

    switch (level)
    {
        case LOG_LEVEL_INFO:
            esp_log_writev(ESP_LOG_INFO, tag, fmt, args);
            break;
        case LOG_LEVEL_WARN:
            esp_log_writev(ESP_LOG_WARN, tag, fmt, args);
            break;
        case LOG_LEVEL_ERROR:
            esp_log_writev(ESP_LOG_ERROR, tag, fmt, args);
            break;
        default:
            esp_log_writev(ESP_LOG_ERROR, tag, "Unknown log level", args);
            break;
    }

    va_end(args);
}