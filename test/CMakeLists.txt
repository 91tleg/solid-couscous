cmake_minimum_required(VERSION 3.16.0)
project(Tests C)

enable_testing()
include(FetchContent)

FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.0
)
FetchContent_MakeAvailable(unity)

include_directories(
    ${CMAKE_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/../src
    ${CMAKE_SOURCE_DIR}/modules
    ${unity_SOURCE_DIR}/src
)


# Unity
set(UNITY_SOURCES ${unity_SOURCE_DIR}/src/Unity.c)

# Modules
set(BUTTON_SOURCES ../src/modules/button/button.c)
set(LCD_SOURCES ../src/modules/lcd/lcd.c)


add_executable(test_button
    ${BUTTON_SOURCES}
    ${UNITY_SOURCES}
    modules/test_button/test_button.c
    modules/test_button/test_main.c
)

add_executable(test_lcd
    ${LCD_SOURCES}
    ${UNITY_SOURCES}
    modules/test_lcd/test_lcd.c
    modules/test_lcd/test_main.c
)


add_test(NAME test_button COMMAND test_button)
add_test(NAME test_lcd COMMAND test_lcd)