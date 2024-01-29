cmake_minimum_required(VERSION 3.2)
project(4004-emulator C)

set (CMAKE_C_STANDARD 99)
set(src_folder "src/")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99")
endif()

file(GLOB PROJECT_SOURCES ${src_folder}/*.c)
file(GLOB PROJECT_HEADERS ${src_folder}/*.h)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

