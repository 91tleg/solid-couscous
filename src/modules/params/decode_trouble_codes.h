#ifndef DECODE_TROUBLE_CODES_H
#define DECODE_TROUBLE_CODES_H

#include <stdint.h>
#include "parameters.h"

union param_type decode_input_switches(uint8_t value);
union param_type decode_io_switches(uint8_t value);
union param_type decode_trouble_code_one(uint8_t value);
union param_type decode_trouble_code_two(uint8_t value);
union param_type decode_trouble_code_three(uint8_t value);

#endif // DECODE_TROUBLE_CODES_H