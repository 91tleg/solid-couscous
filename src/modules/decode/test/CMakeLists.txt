cmake_minimum_required(VERSION 3.16.0)
project(test_decode)

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
    ${CMAKE_SOURCE_DIR}/test_decode_parameters.c
    ${CMAKE_SOURCE_DIR}/test_decode_trouble_codes.c
    ${CMAKE_SOURCE_DIR}/../src/decode_parameters.c
    ${CMAKE_SOURCE_DIR}/../src/decode_trouble_codes.c
)

add_compile_definitions(NDEBUG)

add_test(NAME decode COMMAND unit_tests)
