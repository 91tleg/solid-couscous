#ifndef DECODE_TASK_H
#define DECODE_TASK_H

typedef struct QueueDefinition *QueueHandle_t;

void decode_task_init(QueueHandle_t uart_data_q, QueueHandle_t decoded_data_q);

#endif // DECODE_TASK_H