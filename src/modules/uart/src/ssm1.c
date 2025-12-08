#include "ssm1.h"
#include "drivers/uart/uart.h"
#include "defines.h"

#define GET_MSB(addr) ((uint8_t)(((uint16_t)(addr)) >> 8U))
#define GET_LSB(addr) ((uint8_t)((uint16_t)(addr) & 0xFFU))

static void stop_read(void)
{
    static const uint8_t stop_command[4] = {0x12, 0x00, 0x00, 0x00};
    send_bytes(stop_command, sizeof(stop_command));
}

void read_romid(struct romid_ctx *ctx)
{
    static const uint8_t cmds[2][4] = {
        {0x78, 0xFF, 0xFE, 0x00},
        {0x00, 0xFF, 0xFF, 0x00}};

    if (ctx->done)
    {
        return;
    }

    send_bytes(cmds[ctx->cmd_index], sizeof(cmds[ctx->cmd_index]));
    ctx->cmd_index ^= 1;

    uint8_t buf[4];
    int len = read_bytes(buf, sizeof(buf));

    for (int i = 0; i < len; i++)
    {
        switch (ctx->state)
        {
        case ROMID_WAIT_FF:
            if (buf[i] == 0xFF)
            {
                ctx->state = ROMID_WAIT_FE;
            }
            break;

        case ROMID_WAIT_FE:
            ctx->state = (buf[i] == 0xFE) ? ROMID_ACCEPT_ANY : ROMID_WAIT_FF;
            break;

        case ROMID_ACCEPT_ANY:
            ctx->state = ROMID_GOT_BYTE1;
            break;

        case ROMID_GOT_BYTE1:
            ctx->romid_out[0] = buf[i];
            ctx->state = ROMID_GOT_BYTE2;
            break;

        case ROMID_GOT_BYTE2:
            ctx->romid_out[1] = buf[i];
            ctx->state = ROMID_GOT_BYTE3;
            break;

        case ROMID_GOT_BYTE3:
            ctx->romid_out[2] = buf[i];
            stop_read();
            ctx->done = true;
            ctx->sent = false;
            break;
        }
        if (ctx->done)
        {
            break;
        }
    }
}

void read_data_from_address(struct read_ctx *ctx)
{
    uint8_t read_command[4] = {0x78, GET_MSB(ctx->addr), GET_LSB(ctx->addr), 0x00};

    if (!ctx->sent)
    {
        send_bytes(read_command, sizeof(read_command));
        ctx->sent = true;
        ctx->read_state = READ_WAIT_MSB;
    }

    uint8_t buf[4];
    int len = read_bytes(buf, sizeof(buf));

    for (int i = 0; i < len; i++)
    {
        uint8_t b = buf[i];

        switch (ctx->read_state)
        {
        case READ_WAIT_MSB:
            if (b == read_command[1])
            {
                ctx->read_state = READ_WAIT_LSB;
            }
            break;

        case READ_WAIT_LSB:
            ctx->read_state = (b == read_command[2]) ? READ_WAIT_DATA : READ_WAIT_MSB;
            break;

        case READ_WAIT_DATA:
            ctx->data = b;
            ctx->read_state = READ_WAIT_MSB;
            break;
        }
    }
}

void send_clear_command(struct read_ctx *ctx)
{
    uint8_t clear_command[4] = {0xAA, GET_MSB(ctx->addr), GET_LSB(ctx->addr), 0x00};
    send_bytes(clear_command, sizeof(clear_command));
}