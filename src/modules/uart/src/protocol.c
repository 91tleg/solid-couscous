#include "protocol.h"

#if defined(SSM1)
#include "ssm1.h"

static struct ssm1_parser ssm1_parser_instance;

static size_t parser_feed_adapter(protocol_parser_t parser,
                                  const uint8_t *buf,
                                  size_t len,
                                  struct parsed_msg *out_msgs,
                                  size_t out_cap)
{
    return ssm1_parser_feed((struct ssm1_parser*)parser,
                             buf, len, out_msgs, out_cap);
}

static const protocol_api_t protocol_api = {
    .get_romid_cmd = ssm1_get_romid_command,
    .get_read_cmd = ssm1_get_read_command,
    .get_stop_cmd = ssm1_get_stop_command,
    .get_clear_cmd = ssm1_get_clear_command,
    .parser_feed   = parser_feed_adapter
};

protocol_parser_t protocol_parser_create(void)
{
    return &ssm1_parser_instance;
}

#else
#error "No protocol selected"
#endif

const protocol_api_t* protocol_get_api(void)
{
    return &protocol_api;
}
