cmake_minimum_required(VERSION 3.2)

set(project_name 4004_emulator)
set(src_folder "src/")

project(${project_name})

add_executable(${project_name} main.c ${src_folder}4004_chip.c)
