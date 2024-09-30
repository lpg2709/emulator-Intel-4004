#include "../vendor/unity/unity_internals.h"
#include "./options_parser.c"

void setUp() {}

void tearDown() {}

int main() {
	UNITY_BEGIN();
	RUN_TEST(test_help_mode);
	RUN_TEST(test_version_mode);

	RUN_TEST(test_emulator_default);
	RUN_TEST(test_assembler_mode_long);
	RUN_TEST(test_assembler_mode_short);

	RUN_TEST(test_disassembler_mode_long);
	RUN_TEST(test_disassembler_mode_short);
	RUN_TEST(test_set_output_file_path);
	RUN_TEST(test_set_input_file_path);

	return UNITY_END();
}

