#ifndef DISPLAY_TASK_H
#define DISPLAY_TASK_H

typedef struct QueueDefinition *QueueHandle_t;

void display_task_init(QueueHandle_t display_q);

#endif // DISPLAY_TASK_H