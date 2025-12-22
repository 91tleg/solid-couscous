#pragma once

#define UNUSED(x) ((void)(x))
#define SUPPRESS_UNUSED __attribute__((unused))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))