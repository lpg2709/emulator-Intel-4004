add_subdirectory(vendor/unity)

add_executable(a_scanner ./a_scanner/test_runner.c ../src/a_scanner.c ../src/a_token.c ../src/files.c)
target_link_libraries(a_scanner unity)
target_include_directories(a_scanner PRIVATE ../src)

add_test(NAME a_scanner_test COMMAND a_scanner)

add_executable(options_parser ./options_parser/test_runner.c ../src/options_parser.c)
target_link_libraries(options_parser unity)
target_include_directories(options_parser PRIVATE ../src)

add_test(NAME options_parser COMMAND options_parser)

add_executable(a_token ./a_token/test_runner.c ../src/a_token.c)
target_link_libraries(a_token unity)
target_include_directories(a_token PRIVATE ../src)

add_test(NAME a_token COMMAND a_token)

add_executable(assembler ./assembler/test_runner.c ../src/assembler.c ../src/options_parser.c ../src/a_scanner.c ../src/files.c ../src/a_token.c ../src/a_hash_table.c)
target_link_libraries(assembler unity)
target_include_directories(assembler PRIVATE ../src)

add_test(NAME assembler COMMAND assembler)

file(COPY ${CMAKE_SOURCE_DIR}/roms DESTINATION ${CMAKE_BINARY_DIR}/test)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test/assembled_roms)

