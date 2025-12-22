idf_component_register(
    SRCS
        "src/button.c"
        "src/hd44780.c"
        "src/i2c.c"
        "src/led.c"
        "src/uart.c"

    INCLUDE_DIRS "include"

    PRIV_REQUIRES
        core
        driver
        freertos
)