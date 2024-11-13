#include "../vendor/unity/unity.h"

#include <stdlib.h>
#include <inttypes.h>

#include "../../src/assembler.h"
#include "../../src/options_parser.h"
#include "../../src/error.h"
#include "../../src/files.h"

static void test_assembler_rom_FIN() {
	Options opt = {
		.mode = ASSEMBLER,
		.output_file_path = "assembled_roms/FIN",
		.in_file_path = "./roms/src/FIN.s"
	};

	Error e = assembler(&opt);

	TEST_ASSERT_EQUAL_INT(ERROR_NOT, e);

	long file_size = 0;
	uint8_t *file_content = (uint8_t *) b_read_file(opt.output_file_path, &file_size);
	TEST_ASSERT_EQUAL_INT(1, file_size);

	TEST_ASSERT_EQUAL_INT(0x34, file_content[0]);
	free(file_content);
}

static void test_assembler_rom_JUN() {
	Options opt = {
		.mode = ASSEMBLER,
		.output_file_path = "assembled_roms/JUN",
		.in_file_path = "./roms/src/JUN.s"
	};

	Error e = assembler(&opt);

	TEST_ASSERT_EQUAL_INT(ERROR_NOT, e);

	long file_size = 0;
	uint8_t *file_content = (uint8_t *) b_read_file(opt.output_file_path, &file_size);
	TEST_ASSERT_EQUAL_INT(2, file_size);

	TEST_ASSERT_EQUAL_INT(0x40, file_content[0]);
	TEST_ASSERT_EQUAL_INT(0xF0, file_content[1]);
	free(file_content);
}

