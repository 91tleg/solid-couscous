#ifndef LOG_H
#define LOG_H

#if !defined(NDEBUG)
#include <esp_log.h>
#define LOGI(tag, fmt, ...) ESP_LOGI(tag, fmt, ##__VA_ARGS__)
#define LOGW(tag, fmt, ...) ESP_LOGW(tag, fmt, ##__VA_ARGS__)
#define LOGE(tag, fmt, ...) ESP_LOGE(tag, fmt, ##__VA_ARGS__)
#else
#define LOGI(tag, fmt, ...) ((void)0)
#define LOGW(tag, fmt, ...) ((void)0)
#define LOGE(tag, fmt, ...) ((void)0)
#endif // NDEBUG

#endif // LOG_H