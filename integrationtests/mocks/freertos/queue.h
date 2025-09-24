#ifndef MOCK_QUEUE_H
#define MOCK_QUEUE_H

#include "FreeRTOS.h"

#ifdef __cplusplus
extern "C"
{
#endif

QueueHandle_t xQueueCreate(size_t length, size_t itemSize);
int xQueueSend(QueueHandle_t handle, const void* item, TickType_t timeout);
int xQueueReceive(QueueHandle_t handle, void* out, TickType_t timeout);

#ifdef __cplusplus
}
#endif

#endif // MOCK_QUEUE_H