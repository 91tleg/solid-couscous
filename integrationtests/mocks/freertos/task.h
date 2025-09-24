#ifndef MOCK_TASK_H
#define MOCK_TASK_H

#include "FreeRTOS.h"

#ifdef __cplusplus
extern "C"
{
#endif

int xTaskCreate(TaskFunction_t fn, const char* name, size_t stack, void* params, int prio, void* handle);
void vTaskDelay(TickType_t ticks);

#ifdef __cplusplus
}
#endif

#endif // MOCK_TASK_H