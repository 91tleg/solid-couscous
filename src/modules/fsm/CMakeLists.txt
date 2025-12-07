idf_component_register(
    SRCS
        "src/fsm_task.c"
        "src/fsm.c"

    INCLUDE_DIRS "include"

    PRIV_REQUIRES
        freertos
    
    REQUIRES
        core
)