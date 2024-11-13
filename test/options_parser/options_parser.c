#include "../vendor/unity/unity.h"

#include "../../src/options_parser.h"

#define EXECUTABLE_NAME "4004-emulator"
#define T_ARGC (sizeof(argv) / sizeof(argv[0]))

static void test_help_mode() {
	char *argv[] = { EXECUTABLE_NAME,  "--help" };
	Options options;
	Error err_opt = options_parser(T_ARGC, argv, &options);
	TEST_ASSERT_EQUAL_INT(ERROR_NOT, err_opt);
	TEST_ASSERT_EQUAL_INT(HELP, options.mode);
}

static void test_version_mode() {
	char *argv[] = { EXECUTABLE_NAME, "--version" };
	Options options;
	Error err_opt = options_parser(T_ARGC, argv, &options);
	TEST_ASSERT_EQUAL_INT(ERROR_NOT, err_opt);
	TEST_ASSERT_EQUAL_INT(VERSION, options.mode);
}

static void test_emulator_default() {
	char *argv[] = { EXECUTABLE_NAME };
	Options options;
	Error err_opt = options_parser(T_ARGC, argv, &options);
	TEST_ASSERT_EQUAL_INT(ERROR_NOT, err_opt);
	TEST_ASSERT_EQUAL_INT(EMULATOR, options.mode);
}

static void test_assembler_mode_long() {
	char *argv[] = { EXECUTABLE_NAME, "--assembler" };
	Options options;
	Error err_opt = options_parser(T_ARGC, argv, &options);
	TEST_ASSERT_EQUAL_INT(ERROR_NOT, err_opt);
	TEST_ASSERT_EQUAL_INT(ASSEMBLER, options.mode);
}

static void test_assembler_mode_short() {
	char *argv[] = { EXECUTABLE_NAME, "-a" };
	Options options;
	Error err_opt = options_parser(T_ARGC, argv, &options);
	TEST_ASSERT_EQUAL_INT(ERROR_NOT, err_opt);
	TEST_ASSERT_EQUAL_INT(ASSEMBLER, options.mode);
}

static void test_disassembler_mode_long() {
	char *argv[] = { EXECUTABLE_NAME, "--disassembler" };
	Options options;
	Error err_opt = options_parser(T_ARGC, argv, &options);
	TEST_ASSERT_EQUAL_INT(ERROR_NOT, err_opt);
	TEST_ASSERT_EQUAL_INT(DISASSEMBLER, options.mode);
}

static void test_disassembler_mode_short() {
	char *argv[] = { EXECUTABLE_NAME, "-d" };
	Options options;
	Error err_opt = options_parser(T_ARGC, argv, &options);
	TEST_ASSERT_EQUAL_INT(ERROR_NOT, err_opt);
	TEST_ASSERT_EQUAL_INT(DISASSEMBLER, options.mode);
}

static void test_set_output_file_path() {
	char *argv[] = { EXECUTABLE_NAME, "-o", "test/foo" };
	Options options;
	Error err_opt = options_parser(T_ARGC, argv, &options);
	TEST_ASSERT_EQUAL_INT(ERROR_NOT, err_opt);
	TEST_ASSERT_EQUAL_STRING(argv[2], options.output_file_path);
}

static void test_set_input_file_path() {
	char *argv[] = { EXECUTABLE_NAME, "-e", "test/foo" };
	Options options;
	Error err_opt = options_parser(T_ARGC, argv, &options);
	TEST_ASSERT_EQUAL_INT(ERROR_NOT, err_opt);
	TEST_ASSERT_EQUAL_INT(EMULATOR, options.mode);
	TEST_ASSERT_EQUAL_STRING(argv[2], options.in_file_path);
}
