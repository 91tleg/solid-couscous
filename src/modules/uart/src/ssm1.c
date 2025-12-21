#include "ssm1.h"
#include <string.h>

#define GET_MSB(addr) ((uint8_t)(((uint16_t)(addr)) >> 8U))
#define GET_LSB(addr) ((uint8_t)((uint16_t)(addr) & 0xFFU))

void ssm1_get_romid_command(struct romid_ctx *ctx, uint8_t *cmd)
{
    static const uint8_t cmds[2][4] = {
        {0x78, 0xFF, 0xFE, 0x00},
        {0x00, 0xFF, 0xFF, 0x00}
    };

    memcpy(cmd, cmds[ctx->cmd_index], sizeof(cmds[ctx->cmd_index]));
    ctx->cmd_index ^= 1;
}

void ssm1_get_read_command(struct read_ctx *ctx, uint8_t *cmd)
{
    const uint8_t read_command[4] = {
        0x78, GET_MSB(ctx->addr), GET_LSB(ctx->addr), 0x00
    };
    memcpy(cmd, read_command, sizeof(read_command));
}

void ssm1_get_stop_command(struct read_ctx *ctx, uint8_t *cmd)
{
    const uint8_t stop_command[4] = {
        0x12, GET_MSB(ctx->addr), GET_LSB(ctx->addr), 0x00
    };
    memcpy(cmd, stop_command, sizeof(stop_command));
}

void ssm1_get_clear_command(struct read_ctx *ctx, uint8_t *cmd)
{
    const uint8_t clear_command[4] = {
        0xAA, GET_MSB(ctx->addr), GET_LSB(ctx->addr), 0x00
    };
    memcpy(cmd, clear_command, sizeof(clear_command));
}

size_t ssm1_parser_feed(struct ssm1_parser *p,
                        const uint8_t *buf,
                        size_t len,
                        struct parsed_msg *out_msgs,
                        size_t out_cap)
{
    size_t out_count = 0;

    for (size_t i = 0; i < len; ++i)
    {
        uint8_t b = buf[i];

        switch (p->state)
        {
        case P_WAIT:
            p->state = (b == 0xFF) ? P_MAYBE_ROMID_FE : P_READ_LSB; 
            break;

        case P_MAYBE_ROMID_FE:
            p->state = (b == 0xFE) ? P_ROMID_SKIP : P_READ_DATA;
            break;

        case P_ROMID_SKIP:
            p->rom_index = 0;
            p->state = P_ROMID_BYTES;
            break;

        case P_ROMID_BYTES:
            p->rom_buf[p->rom_index++] = b;
            if (p->rom_index == 3)
            {
                out_msgs->type = MSG_TYPE_ROMID;
                memcpy(out_msgs->u.rom.romid, p->rom_buf, 3);
                p->state = P_WAIT;
            }
            break;

        case P_READ_LSB:
            p->state = P_READ_DATA;
            break;

        case P_READ_DATA:
            if (out_count < out_cap)
            {
                struct parsed_msg *m = &out_msgs[out_count++];
                m->type = MSG_TYPE_READ;
                m->u.read.value = b;
            }
            p->state = P_WAIT;
            break;

        default:
            p->state = P_WAIT;
            break;
        }
    }

    return out_count;
}
