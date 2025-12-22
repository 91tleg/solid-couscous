cmake_minimum_required(VERSION 3.16.0)
project(test_uart)

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
    ${CMAKE_SOURCE_DIR}/test_ssm1.c
    ${CMAKE_SOURCE_DIR}/test_addr_map.c
    ${CMAKE_SOURCE_DIR}/../src/ssm1.c
    ${CMAKE_SOURCE_DIR}/../src/addr_map.c
)

add_compile_definitions(NDEBUG)

add_test(NAME uart COMMAND unit_tests)
