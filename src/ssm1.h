/*
 * http://www.4bc.org/vanagon/SSM_params.html
 * http://www.alcyone.org.uk/ssm/ecureverse.html
 * https://github.com/P1kachu/ssm1-gc8
 */

#ifndef SSM1_H
#define SSM1_H

#include <stdint.h>

/**
 * @brief Retrieves the ROM ID from the ECU.
 * @param buffer Pointer to a buffer where the ROM ID will be stored.
 */
void get_romid(uint8_t *buffer);

/**
 * @brief Sends a command to read an ECU memory address.
 * @param addr The 16-bit ECU memory address.
 */
uint8_t read_data_from_address(uint16_t addr);

/**
 * @brief Sends a command to clear an ECU memory address.
 * @param addr The 16-bit memory address in the ECU to be cleared.
 */
void send_clear_command(uint16_t addr);

#endif // SSM1_H