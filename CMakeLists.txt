cmake_minimum_required(VERSION 3.2)

set(project_name 4004-emulator)
set(src_folder "src/")

project(${project_name})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -std=c99")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()


file(GLOB SOURCES ${src_folder}/*.c)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/roms DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${project_name} main.c ${SOURCES})
