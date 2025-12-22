idf_component_register(
    SRCS
        "src/button_task.c"
        "src/button.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES
        driver
        core
        freertos
)