cmake_minimum_required(VERSION 3.16.0)
project(integration_tests C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mocks)
set(BUTTON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/modules/button)
set(STATE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/../src/modules/state_machine)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# FreeRTOS
add_library(mock_freertos STATIC
    ${MOCK_DIR}/freertos/queue.cpp
    ${MOCK_DIR}/freertos/task.cpp
)
target_include_directories(mock_freertos PUBLIC ${MOCK_DIR} ${INCLUDE_DIR})
target_link_libraries(mock_freertos pthread)

# Drivers
add_library(mock_drivers INTERFACE)
target_include_directories(mock_drivers INTERFACE ${MOCK_DIR} ${INCLUDE_DIR})

# Core
add_library(mock_core INTERFACE)
target_include_directories(mock_core INTERFACE ${MOCK_DIR} ${INCLUDE_DIR})

# Tasks
add_library(mock_native STATIC
    ${MOCK_DIR}/button/button_task.c
    ${MOCK_DIR}/state_machine/state_machine_task.c
)
target_include_directories(mock_native PUBLIC ${MOCK_DIR} ${INCLUDE_DIR})


add_library(button_task STATIC
    ${BUTTON_DIR}/button_task.c
)
target_include_directories(button_task PUBLIC
    ${BUTTON_DIR}
    ${INCLUDE_DIR}
    ${MOCK_DIR}
)

add_library(state_machine_task STATIC
    ${STATE_DIR}/state_machine_task.c
)
target_include_directories(state_machine_task PUBLIC
    ${STATE_DIR}
    ${BUTTON_DIR}
    ${INCLUDE_DIR}
    ${MOCK_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Test executable
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test_*.c)
list(APPEND TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test_main.c)

add_executable(integration_tests_runner ${TEST_SOURCES})

target_include_directories(integration_tests_runner PRIVATE
    ${BUTTON_DIR}
    ${STATE_DIR}
    ${INCLUDE_DIR}
    ${MOCK_DIR}
)

target_link_libraries(integration_tests_runner
    mock_freertos
    mock_drivers
    mock_native
    mock_core
    button_task
    state_machine_task
    pthread
)

add_test(NAME integration_tests_runner COMMAND integration_tests_runner)