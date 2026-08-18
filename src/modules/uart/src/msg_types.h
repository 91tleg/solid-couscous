#pragma once
#include <stdint.h>

typedef enum
{
    MSG_TYPE_ROMID,
    MSG_TYPE_READ
} msg_type_e;

struct parsed_msg
{
    msg_type_e type;
    union {
        struct { uint8_t romid[3]; } rom; // MSG_TYPE_ROMID
        struct { uint8_t value; } read;   // MSG_TYPE_READ
    } u;
};
