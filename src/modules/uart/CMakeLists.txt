idf_component_register(
    SRCS
        "src/uart_task.c"
        "src/addr_map.c"
        "src/ssm1.c"

    INCLUDE_DIRS "include"

    REQUIRES
        core

    PRIV_REQUIRES
        freertos
        drivers
)