#ifndef LOG_H
#define LOG_H

typedef enum {
    LOG_LEVEL_NONE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
} log_level_e;

/**
 * @brief Set the log level.
 *
 * @param level Log level.
 */
void log_level_set(log_level_e level);

/**
 * @brief Log a formatted message.
 *
 * @param level Log level.
 * @param tag   Component tag string.
 * @param fmt   printf-style format string.
 * @param ...   Arguments.
 */
void log_message(log_level_e level, const char *tag, const char *fmt, ...);

#define LOGI(tag, fmt, ...) log_message(LOG_LEVEL_INFO, tag, fmt, ##__VA_ARGS__)
#define LOGW(tag, fmt, ...) log_message(LOG_LEVEL_WARN, tag, fmt, ##__VA_ARGS__)
#define LOGE(tag, fmt, ...) log_message(LOG_LEVEL_ERROR, tag, fmt, ##__VA_ARGS__)

#endif // LOG_H