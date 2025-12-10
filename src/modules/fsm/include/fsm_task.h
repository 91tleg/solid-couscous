#ifndef FSM_TASK_H
#define FSM_TASK_H

typedef struct QueueDefinition *QueueHandle_t;
typedef struct tskTaskControlBlock *TaskHandle_t;

void fsm_task_init(QueueHandle_t btn_evt_q, TaskHandle_t uart_task_h);

#endif // FSM_TASK_H
