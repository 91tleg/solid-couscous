#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "button.h"
#include "state_machine.h"

void app_main()
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BTN_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&io_conf);

    xTaskCreate(
        button_task, 
        "ButtonTask", 
        2048, 
        NULL, 
        2, 
        NULL);
    
    xTaskCreate(
        state_machine_task, 
        "State Machine Task", 
        2048, 
        NULL, 
        5, 
        NULL);
}