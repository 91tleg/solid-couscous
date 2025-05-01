#pragma once
#include <stdint.h>

#define UNUSED(x) (void)(x)
#define SUPPRESS_UNUSED __attribute__((unused))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define GET_HIGH_NIB(x) (((uint8_t)(x)) & 0xF0U)
#define GET_LOW_NIB(x) (((uint8_t)((x)) << 4U) & 0xF0U)
#define GET_MSB(addr) (((uint16_t)(addr)) >> 8U)
#define GET_LSB(addr) (((uint16_t)(addr)) & 0xFFU)