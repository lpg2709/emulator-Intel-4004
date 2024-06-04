add_subdirectory(vendor/unity)

add_executable(assembler ./assembler/test_runner.c ../src/a_scanner.c ../src/a_token.c)
target_link_libraries(assembler unity)
target_include_directories(assembler PRIVATE ../src)

add_test(NAME AssemblerTest COMMAND assembler)
