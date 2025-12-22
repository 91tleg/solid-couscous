#ifndef DECODE_TROUBLE_CODES_H
#define DECODE_TROUBLE_CODES_H

#include <stdint.h>
#include "parameters.h"

struct input_switches decode_input_switches(uint8_t value);
struct io_switches decode_io_switches(uint8_t value);
struct trouble_code_one decode_trouble_code_one(uint8_t value);
struct trouble_code_two decode_trouble_code_two(uint8_t value);
struct trouble_code_three decode_trouble_code_three(uint8_t value);

#endif // DECODE_TROUBLE_CODES_H