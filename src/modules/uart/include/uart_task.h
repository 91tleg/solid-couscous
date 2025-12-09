#ifndef UART_TASK_H
#define UART_TASK_H

typedef struct tskTaskControlBlock *TaskHandle_t;
typedef struct QueueDefinition *QueueHandle_t;

TaskHandle_t uart_task_init(QueueHandle_t uart_data_q);

#endif // UART_TASK_H