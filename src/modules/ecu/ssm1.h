#ifndef SSM1_H
#define SSM1_H

#include <stdint.h>
#include <stdbool.h>
#include "state_machine.h"

typedef enum
{
    ROMID_WAIT_FF,
    ROMID_WAIT_FE,
    ROMID_ACCEPT_ANY,
    ROMID_GOT_BYTE1,
    ROMID_GOT_BYTE2,
    ROMID_GOT_BYTE3
} romid_state_e;

typedef enum
{
    READ_WAIT_MSB,
    READ_WAIT_LSB,
    READ_WAIT_DATA
} read_state_e;

struct romid_ctx
{
    romid_state_e state;
    uint8_t cmd_index;
    uint8_t pos;
    uint8_t romid_out[3];
    bool done;
    bool sent;
};

struct read_ctx
{
    read_state_e read_state;
    bool used;
    state_e state;
    uint16_t addr;
    uint8_t data;
    bool sent;
};

/**
 * @brief Sends a command to read the ECU romid.
 * @param ctx Pointer to a romid_ctx struct tracking ROM ID read state.
 */
void read_romid(struct romid_ctx *ctx);

/**
 * @brief Sends a command to read an ECU memory address.
 * @param ctx Pointer to a read_ctx struct tracking the address and storing the result.
 */
void read_data_from_address(struct read_ctx *ctx);

/**
 * @brief Sends a command to clear an ECU memory address.
 * @param ctx Pointer to a read_ctx struct specifying the address to clear.
 */
void send_clear_command(struct read_ctx *ctx);

#endif // SSM1_H