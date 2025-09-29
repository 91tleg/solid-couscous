#include "queue.h"
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <cstring>

struct Queue
{
    std::queue<std::vector<uint8_t>> buffer;
    std::mutex mtx;
    std::condition_variable cv;
    size_t itemSize;
    size_t length;
};

QueueHandle_t xQueueCreate(size_t length, size_t itemSize)
{
    auto *q = new Queue();
    q->length = length;
    q->itemSize = itemSize;
    return q;
}

int xQueueSend(QueueHandle_t handle, const void *item, TickType_t timeout)
{
    auto *q = (Queue *)handle;
    std::unique_lock<std::mutex> lock(q->mtx);
    if (q->buffer.size() >= q->length)
    {
        return pdFAIL;
    }
    std::vector<uint8_t> data((uint8_t *)item, (uint8_t *)item + q->itemSize);
    q->buffer.push(data);
    lock.unlock();
    q->cv.notify_one();
    return pdPASS;
}

int xQueueReceive(QueueHandle_t handle, void *out, TickType_t timeout)
{
    auto *q = (Queue *)handle;
    std::unique_lock<std::mutex> lock(q->mtx);

    if (timeout == portMAX_DELAY)
    {
        q->cv.wait(lock, [&]
                   { return !q->buffer.empty(); });
    }
    else
    {
        if (!q->cv.wait_for(lock, std::chrono::milliseconds(timeout), [&]
                            { return !q->buffer.empty(); }))
        {
            return pdFAIL;
        }
    }

    auto data = q->buffer.front();
    memcpy(out, data.data(), q->itemSize);
    q->buffer.pop();
    return pdPASS;
}