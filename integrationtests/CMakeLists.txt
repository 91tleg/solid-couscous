cmake_minimum_required(VERSION 3.16.0)
project(integration_tests C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/modules)
set(UNITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../test/unity/src)

# Unity
add_library(unity STATIC
    ${UNITY_DIR}/unity.c
)
target_include_directories(unity PUBLIC
    ${UNITY_DIR}
)

# FreeRTOS
add_library(mock_freertos STATIC
    ${MOCK_DIR}/freertos/queue.cpp
    ${MOCK_DIR}/freertos/task.cpp
)
target_include_directories(mock_freertos PUBLIC 
    ${MOCK_DIR}
    ${INCLUDE_DIR}
)
target_link_libraries(mock_freertos pthread)

# Core
add_library(mock_core INTERFACE)
target_include_directories(mock_core INTERFACE
    ${MOCK_DIR}
    ${INCLUDE_DIR}
)

# Modules
add_library(mock_modules STATIC
    ${MOCK_DIR}/button/button.c
    ${MOCK_DIR}/state_machine/state_machine.c
)
target_include_directories(mock_modules PUBLIC
    ${MOCK_DIR}
    ${INCLUDE_DIR}
)

# Button task
add_library(button_task STATIC
    ${SRC_DIR}/button/button_task.c
)
target_include_directories(button_task PUBLIC
    ${MOCK_DIR}
    ${INCLUDE_DIR}
    ${SRC_DIR}/button
)

# State machine task
add_library(state_machine_task STATIC
    ${SRC_DIR}/state_machine/state_machine_task.c
)
target_include_directories(state_machine_task PUBLIC
    ${MOCK_DIR}
    ${INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${SRC_DIR}/state_machine
)

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test_*.c)
list(APPEND TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test_main.c)

add_executable(integration_tests_runner
    ${TEST_SOURCES}
)

target_include_directories(integration_tests_runner PRIVATE
    ${MOCK_DIR}
    ${INCLUDE_DIR}
    ${UNITY_DIR}
    ${SRC_DIR}/button
    ${SRC_DIR}/state_machine
)

target_link_libraries(integration_tests_runner
    mock_freertos
    mock_modules
    mock_core
    button_task
    state_machine_task
    pthread
    unity
)

add_test(NAME integration_tests_runner COMMAND integration_tests_runner)