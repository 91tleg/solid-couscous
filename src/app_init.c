#include "app_init.h"
#include "button_driver.h"
#include "i2c.h"
#include "hd44780.h"
#include "led.h"
#include "uart.h"

#include "assert_handler.h"

#include "fsm_states.h"
#include "fsm_data.h"
#include "uart_data.h"
#include "button_event.h"

#include "uart_task.h"
#include "fsm_task.h"
#include "display_task.h"
#include "decode_task.h"
#include "button_task.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

static QueueHandle_t button_isr_queue = NULL;

static QueueHandle_t display_queue = NULL;

static TaskHandle_t uart_task_handle = NULL;
static QueueHandle_t uart_data_queue = NULL;

static QueueHandle_t button_event_queue = NULL;

static i2c_master_bus_handle_t bus_handle = NULL;
static i2c_master_dev_handle_t i2c_handle = NULL;

static void drivers_init(void)
{
    button_isr_queue = xQueueCreate(10, sizeof(struct button_isr));
    ASSERT(button_isr_queue != NULL);

    button_driver_init(button_isr_queue);

    i2c_driver_init(&bus_handle, &i2c_handle);
    
    led_driver_init();
    uart_driver_init();
    hd44780_driver_init(i2c_handle);
}

static void tasks_init(void)
{
    display_queue = xQueueCreate(20, sizeof(struct fsm_data));
    ASSERT(display_queue != NULL);

    uart_data_queue = xQueueCreate(20, sizeof(struct uart_data));
    ASSERT(uart_data_queue != NULL);

    button_event_queue = xQueueCreate(20, sizeof(button_event_e));
    ASSERT(button_event_queue != NULL);

    uart_task_handle = uart_task_init(uart_data_queue);
    display_task_init(display_queue);
    decode_task_init(uart_data_queue, display_queue);
    button_task_init(button_isr_queue, button_event_queue);
    fsm_task_init(button_event_queue, uart_task_handle);
}

void app_init(void)
{
    drivers_init();
    tasks_init();
}