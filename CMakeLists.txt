cmake_minimum_required(VERSION 3.2)
project(4004-emulator C)

set (CMAKE_C_STANDARD 99)
set(src_folder "src/")

add_subdirectory(test)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()


if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99")
endif()

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS ${src_folder}/*.c)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE "DEBUG=$<CONFIG:Debug>")

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

