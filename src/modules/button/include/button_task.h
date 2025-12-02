#ifndef BUTTON_TASK_H
#define BUTTON_TASK_H

typedef struct QueueDefinition *QueueHandle_t;

void button_task_init(QueueHandle_t isr_q, QueueHandle_t event_q);

#endif // BUTTON_TASK_H