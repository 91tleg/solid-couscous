#ifndef SSM1_H
#define SSM1_H

#include "fsm_states.h"
#include <stdint.h>
#include <stddef.h>

typedef enum
{
    P_WAIT,           // start state: FF (ROMID start) or MSB (read)
    P_MAYBE_ROMID_FE, // got 0xFF, expect 0xFE next for ROMID
    P_ROMID_SKIP,     // Junk byte
    P_ROMID_BYTES,    // collecting next 3 bytes for ROMID
    P_READ_MSB,       // collecting MSB of read frame
    P_READ_LSB,       // collecting LSB of read frame
    P_READ_DATA       // collecting DATA of read frame
} parser_state_e;

struct ssm1_parser
{
    parser_state_e state;
    uint8_t rom_buf[3];
    uint8_t rom_index;
    uint8_t tmp_msb;
    uint8_t tmp_lsb;
};

struct romid_ctx {
    uint8_t cmd_index;   // 0 or 1
};

struct read_ctx {
    fsm_state_e state;
    uint16_t addr;
};

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

void ssm1_get_romid_command(struct romid_ctx *ctx, uint8_t *cmd);
void ssm1_get_read_command(struct read_ctx *ctx, uint8_t *cmd);
void ssm1_get_stop_command(struct read_ctx *ctx, uint8_t *cmd);
void ssm1_get_clear_command(struct read_ctx *ctx, uint8_t *cmd);

/**
 * @brief Feed UART bytes into the SSM1 parser state machine.
 *
 * The parser maintains internal state between calls, allowing
 * partial frames, stream data, or chunked input.
 *
 * @param p          Pointer to the parser state machine.
 * @param buf        Pointer to the input byte buffer.
 * @param len        Num of bytes available in @p buf.
 * @param out_msgs   Array parsed messages is written to.
 * @param out_cap    Num of messages @p out_msgs can hold.
 *
 * @return Num of parsed messages written to @p out_msgs.
 */
size_t ssm1_parser_feed(struct ssm1_parser *p,
                        const uint8_t *buf,
                        size_t len,
                        struct parsed_msg *out_msgs,
                        size_t out_cap);

#endif // SSM1_H
