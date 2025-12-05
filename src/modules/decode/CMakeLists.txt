idf_component_register(
    SRCS
        "src/decode_task.c"
        "src/decode_parameters.c"
        "src/decode_trouble_codes.c"

    INCLUDE_DIRS
        "include"

    PRIV_REQUIRES
        freertos

    REQUIRES
        core
)