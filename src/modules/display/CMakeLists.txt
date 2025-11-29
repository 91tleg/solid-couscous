idf_component_register(
    SRCS
        "src/display_task.c"
        "src/display.c"

    INCLUDE_DIRS "include"

    PRIV_REQUIRES
        fsm
        core
        drivers
        freertos
)