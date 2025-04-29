#ifndef DECODE_TROUBLE_CODES_H
#define DECODE_TROUBLE_CODES_H

#include <stdint.h>
#include "parameters.h"

void decode_input_switches(uint8_t value, struct input_switches *dtc);
void decode_io_switches(uint8_t value, struct io_switches *dtc);
void decode_trouble_code_one(uint8_t value, struct trouble_code_one *dtc);
void decode_trouble_code_two(uint8_t value, struct trouble_code_two *dtc);
void decode_trouble_code_three(uint8_t value, struct trouble_code_three *dtc);

#endif // DECODE_TROUBLE_CODES_H