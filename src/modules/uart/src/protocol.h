#pragma once
#include "ctx.h"
#include "msg_types.h"
#include <stddef.h>
#include <stdint.h>

typedef void *protocol_parser_t;

typedef struct {
  void (*get_romid_cmd)(struct romid_ctx *, uint8_t *);
  void (*get_read_cmd)(struct read_ctx *, uint8_t *);
  void (*get_stop_cmd)(struct read_ctx *, uint8_t *);
  void (*get_clear_cmd)(struct read_ctx *, uint8_t *);
  size_t (*parser_feed)(protocol_parser_t parser, const uint8_t *buf,
                        size_t len, struct parsed_msg *out_msgs,
                        size_t out_cap);
} protocol_api_t;

const protocol_api_t *protocol_get_api(void);
