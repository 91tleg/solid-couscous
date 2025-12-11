# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

file(GLOB_RECURSE APP_SOURCES ${CMAKE_SOURCE_DIR}/src/*.*)

foreach(file ${APP_SOURCES})
    if(file MATCHES ".*/test/.*")
        list(REMOVE_ITEM APP_SOURCES ${file})
    endif()
endforeach()

idf_component_register(SRCS ${APP_SOURCES})