#include "log.h"
#include <esp_log.h>
#include <stdarg.h>
#include <stdio.h>

static log_level_e current_log_level = LOG_LEVEL_ERROR;

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

    char buf[64];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    switch(level)
    {
        case LOG_LEVEL_INFO:  
            ESP_LOGI(tag, "%s", buf); 
            break;
        case LOG_LEVEL_WARN:  
            ESP_LOGW(tag, "%s", buf); 
            break;
        case LOG_LEVEL_ERROR: 
            ESP_LOGE(tag, "%s", buf); 
            break;
        default: 
            break;
    }
}

void log_init(void)
{
    #if defined(BOARD_ESP32DEV)
    log_level_set(LOG_LEVEL_ERROR);
    #else
    log_level_set(LOG_LEVEL_NONE);
    #endif
}