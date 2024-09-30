add_subdirectory(vendor/unity)

add_executable(assembler ./assembler/test_runner.c ../src/a_scanner.c ../src/a_token.c)
target_link_libraries(assembler unity)
target_include_directories(assembler PRIVATE ../src)

add_test(NAME assembler_test COMMAND assembler)

add_executable(options_parser ./options_parser/test_runner.c ../src/options_parser.c)
target_link_libraries(options_parser unity)
target_include_directories(options_parser PRIVATE ../src)

add_test(NAME options_parser COMMAND options_parser)
