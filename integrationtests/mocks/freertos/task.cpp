#include "task.h"
#include <pthread.h>
#include <unistd.h>

extern "C" int xTaskCreate(TaskFunction_t fn, const char* name, size_t stack, void* params, int prio, void* handle)
{
    pthread_t thread;
    pthread_create(&thread, nullptr, (void*(*)(void*))fn, params);
    pthread_detach(thread);
    return pdPASS;
}

extern "C" void vTaskDelay(TickType_t ticks)
{
    usleep(ticks * 1000); // ms
}
