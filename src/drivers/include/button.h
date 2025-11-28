#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

typedef struct QueueDefinition * QueueHandle_t;

struct button_isr
{
    int level;
    int64_t timestamp_us; // When the press occurred
};

/**
 * @brief Initialize the button gpio.
 * @param isr_queue Queue for the gpio ISR.
 */
void button_driver_init(QueueHandle_t isr_queue);

/**
 * @brief Read the button gpio state.
 * @return 0 if button is pressed, else 1.
 */
int button_driver_read(void);

#endif // BUTTON_H