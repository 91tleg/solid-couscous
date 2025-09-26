#ifndef MOCK_FREERTOS_H
#define MOCK_FREERTOS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

typedef void* QueueHandle_t;
typedef unsigned int TickType_t;
typedef void (*TaskFunction_t)(void*);

#define pdPASS              (1)
#define pdFAIL              (0)
#define portMAX_DELAY       (0xFFFFFFFF)
#define pdMS_TO_TICKS(ms)   (ms)
#define portTICK_PERIOD_MS  (1)

#ifdef __cplusplus
}
#endif

#endif // MOCK_FREERTOS_H