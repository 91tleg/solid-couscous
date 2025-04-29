#ifndef DECODE_BINARY_H
#define DECODE_BINARY_H

#include <stdint.h>
#include "parameters.h"

void decode_input_switches(uint8_t value, struct input_switches *buf);
void decode_io_switches(uint8_t value, struct io_switches *buf);
void decode_trouble_code_one(uint8_t value, struct trouble_code_one *buf);
void decode_trouble_code_two(uint8_t value, struct trouble_code_two *buf);
void decode_trouble_code_three(uint8_t value, struct trouble_code_three *buf);

#endif // DECODE_BINARY_H