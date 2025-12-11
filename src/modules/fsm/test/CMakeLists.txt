cmake_minimum_required(VERSION 3.16.0)
project(test_fsm)

enable_testing()

set(UNITY_PATH ${CMAKE_SOURCE_DIR}/../../../../test/unity/src)

include_directories(
    ${UNITY_PATH}
    ${CMAKE_SOURCE_DIR}/../../../core/include
    ${CMAKE_SOURCE_DIR}/../src
)

add_executable(unit_tests
    ${UNITY_PATH}/unity.c
    ${CMAKE_SOURCE_DIR}/test_main.c
    ${CMAKE_SOURCE_DIR}/test_fsm.c
    ${CMAKE_SOURCE_DIR}/../src/fsm.c
)

add_compile_definitions(NDEBUG)

add_test(NAME fsm COMMAND unit_tests)
